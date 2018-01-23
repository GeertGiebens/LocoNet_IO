
;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;*  32 in-uitgangen met keuze uit:                                         *
;*  - Servo uitgangen                                                      *
;*  - logische uitgangen (5V Relais modules)                               *
;*  - logische uitgangen puls (5V Relais modules voor wisselspoel)         *
;*  - LED uitgangen (Fade in Fade out)                                     *
;*  - bezetmelders (massa)                                                 *
;*  - schakelaar; drukknop; enz... (met debouncing)                        *
;*  - mogelijke toekomstige uitbreidingen                                  *
;*  - mogelijkheid om toetsenbord en LCD display aan te sluiten            *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 23-07-2017                      Versie LOCONET IO V1.1           *
;*                                                                         *
;* Filenaam: LOCONET_IO_LED.INC                                            *
;*                                                                         *
;***************************************************************************

    #DEFINE KNIPPERFREQ 30     ;Knipperfrequentie LED's = (60/KNIPPERFREQ)/2  --> 1Hz
    #DEFINE KNIPPERBIT LED_FLAG,0

;**********************************************************
;*******  LED NAAR POORT PROGRAM  *************************
;**********************************************************

MACRO_LED MACRO LEDNR, POORTLED, POORTLEDBIT

	LOCAL ML_END, ML_SPR10

	BTFSS ((LOW TBL_FUNCTIE)+(LEDNR-1)),6,BANKED	;is poortfunctie = LED?
	BRA ML_END	                    				;  NEE: volgende poort	

	MOVF ((LOW TBL_VAR1) +(LEDNR-1)),W,BANKED   	;is  VAR1 > TMR3H dan LED aan
;    RLNCF WREG,W  
	CPFSLT TMR3H  
	BRA ML_SPR10
	BSF POORTLED,POORTLEDBIT  		            	
	BRA ML_END
ML_SPR10
	BCF POORTLED,POORTLEDBIT                        ;   anders LED aan
ML_END
	NOP
    ENDM


;**********************************************************

LED
	MOVLW HIGH TBL_FUNCTIE		;selecteer bank waar tabellen staan

	
        MACRO_LED 1,POORT1
        MACRO_LED 2,POORT2
        MACRO_LED 3,POORT3
        MACRO_LED 4,POORT4
        MACRO_LED 5,POORT5
        MACRO_LED 6,POORT6
        MACRO_LED 7,POORT7
        MACRO_LED 8,POORT8
        MACRO_LED 9,POORT9
        MACRO_LED 10,POORT10
        MACRO_LED 11,POORT11
        MACRO_LED 12,POORT12
        MACRO_LED 13,POORT13
        MACRO_LED 14,POORT14
        MACRO_LED 15,POORT15
        MACRO_LED 16,POORT16
        MACRO_LED 17,POORT17
        MACRO_LED 18,POORT18
        MACRO_LED 19,POORT19
        MACRO_LED 20,POORT20
        MACRO_LED 21,POORT21
        MACRO_LED 22,POORT22
        MACRO_LED 23,POORT23
        MACRO_LED 24,POORT24
        MACRO_LED 25,POORT25
        MACRO_LED 26,POORT26
        MACRO_LED 27,POORT27
        MACRO_LED 28,POORT28
    IF DEBUG == 0
        MACRO_LED 29,POORT29
        MACRO_LED 30,POORT30
    ENDIF


	RETURN


;**********************************************************
;******* UPDATE LED  **************************************
;**********************************************************

;WAARDEN BINAIR b'76543210'
;                    |||||
;                    ||||b=1 LED in FADE MODE (LED is bezig op te lichten of te doven)
;                    |||b=0  LED staat in MIN stand         b=1 LED staat in MAX stand
;                    ||b=0   Gewenste stand LED= MIN        b=1 Gewenste stand LED=MAX
;                    |b=0    Geen pulserende LED            b=1 Wel pulserende LED 
;                    b=0     Ontvangen gewenste stand = MIN b=1 Ontvangen gewenste stand = MAX      

;CONST1 = MIN waarde LED intensiteit 0-X1
;CONST2 = MAX waarde LED intensiteit X2-255  (waarbij X2>X1)
;CONST3 = Snelheid van Fade In-OUT 1-127 waarbij 1 traag is
;VAR1 = Huidige intensiteit LED


MACRO_FADE_LED	MACRO LED_NR	

	LOCAL MFD_END, MFD_SPR5, MFD_SPR8, MFD_SPR10,MFD_SPR20,MFD_SPR30,MFD_SPR15,MFD_SPR40,MFD_SPR45


    	BTFSS ((LOW TBL_FUNCTIE)+(LED_NR-1)),6,BANKED	;Is poortfunctie LED?
    	BRA MFD_END				                    	;  NEE: dan volgende poort

        BTFSC ((LOW TBL_BINAIR)+(LED_NR-1)),4,BANKED    ;IS ontvangen gewenste stand LED = 0 dan gewenste stand LED = 0
        BRA MFD_SPR5
        BCF ((LOW TBL_BINAIR)+(LED_NR-1)),2,BANKED
        BRA MFD_SPR8
MFD_SPR5
        BSF ((LOW TBL_BINAIR)+(LED_NR-1)),2,BANKED 
        BTFSS ((LOW TBL_BINAIR)+(LED_NR-1)),3,BANKED     ;Is ontvangen gewenste stand LED = 1 en LED moet KNIPPEREN dan gewenste stand LED = afhankelijk van KNIPPERBIT
        BRA MFD_SPR8
        BTFSC KNIPPERBIT  
        BCF ((LOW TBL_BINAIR)+(LED_NR-1)),2,BANKED 
MFD_SPR8
    	BTFSS ((LOW TBL_BINAIR)+(LED_NR-1)),0,BANKED	    ;Is LED in FADE MODE?
    	BRA MFD_SPR10

    	BTFSS ((LOW TBL_BINAIR)+(LED_NR-1)),2,BANKED 	;  JA: is gewenste LED stand MAX
    	BRA MFD_SPR20

        BCF STATUS,C
    	MOVF ((LOW TBL_CONST3)+(LED_NR-1)),W,BANKED	    ;      JA: verhoog VAR1 met waarde CONST3
    	ADDWF ((LOW TBL_VAR1)+(LED_NR-1)),F,BANKED   	;          Is VAR1 >= MAX dan VAR1=MAX en einde Fade IN	naar MAX	 
    	BC MFD_SPR15
    	MOVF ((LOW TBL_VAR1)+(LED_NR-1)),W,BANKED
    	CPFSLT ((LOW TBL_CONST2)+(LED_NR-1)),BANKED
    	BRA MFD_END
MFD_SPR15
    	MOVFF (TBL_CONST2+(LED_NR-1)),(TBL_VAR1+(LED_NR-1))
    	BCF ((LOW TBL_BINAIR)+(LED_NR-1)),0,BANKED
    	BSF ((LOW TBL_BINAIR)+(LED_NR-1)),1,BANKED
	    BRA MFD_END

MFD_SPR20
        BCF STATUS,C
    	MOVF ((LOW TBL_CONST3)+(LED_NR-1)),W,BANKED  	;      NEE: verlaag VAR1 met waarde CONST3
    	SUBWF ((LOW TBL_VAR1)+(LED_NR-1)),F,BANKED	    ;          Is VAR1 <= MIN dan VAR1=MIN en einde Fade OUT naar MIN
    	BNC MFD_SPR30
    	MOVF ((LOW TBL_VAR1)+(LED_NR-1)),W,BANKED
    	CPFSGT ((LOW TBL_CONST1)+(LED_NR-1)),BANKED
    	BRA MFD_END
MFD_SPR30
    	MOVFF (TBL_CONST1+(LED_NR-1)),(TBL_VAR1+(LED_NR-1))
    	BCF ((LOW TBL_BINAIR)+(LED_NR-1)),0,BANKED
    	BCF ((LOW TBL_BINAIR)+(LED_NR-1)),1,BANKED
    	BRA MFD_END	

MFD_SPR10
        BTFSC ((LOW TBL_BINAIR)+(LED_NR-1)),2,BANKED      ;is de gewenste stand MIN en staat LED op MIN dan volgende poort  
        BRA MFD_SPR40
        BTFSS ((LOW TBL_BINAIR)+(LED_NR-1)),1,BANKED
        BRA MFD_END
        BRA MFD_SPR45	
MFD_SPR40
        BTFSC ((LOW TBL_BINAIR)+(LED_NR-1)),1,BANKED      ; is de gewenste stand MAX en staat LED MAX dan volgende poort
        BRA MFD_END
MFD_SPR45                                                  ; INDIEN Anders doe:
        BSF ((LOW TBL_BINAIR)+(LED_NR-1)),0,BANKED        ;   geef aan dat LED in Fade mode is

MFD_END
    	NOP
        ENDM


UPDATE_LED
        DECFSZ KNIPPERTEL,F           ;verminder knipperteller als knipperteller=0 dan reset teller en toggle knipperbit
        BRA UL_SPR10
        MOVLW KNIPPERFREQ
        MOVWF KNIPPERTEL  
        BTG KNIPPERBIT

		
		#DEFINE TEST_BELGISCH_SEIN 1
IF TEST_BELGISCH_SEIN == 1

		;POORT1-6 = LED
		;POORT7 = NOP
		

        BCF TRISA,7                              ;zet TRIS BIT register op uitgang

		MOVLW 14 ;ROOD
	    BTFSC ((LOW TBL_BINAIR)+0),4,BANKED
		MOVLW 28 ;GROEN
	    BTFSC ((LOW TBL_BINAIR)+1),4,BANKED
		MOVLW 42 ;ORANJE/ORANJE
	    BTFSC ((LOW TBL_BINAIR)+2),4,BANKED	
		MOVLW 56 ;GROEN/ORANJE
	    BTFSC ((LOW TBL_BINAIR)+3),4,BANKED	
		MOVLW 80 ;GROEN/ORANJE
	    BTFSC ((LOW TBL_BINAIR)+4),4,BANKED			
		MOVLW 94 ;ROOD/WIT
		
		BCF INTCON,7		;Geen INT
		
		BCF POORT7		;1
		GOTO $+1		;3
		GOTO $+1		;5
		GOTO $+1		;7
		GOTO $+1		;9
		GOTO $+1		;11
		GOTO $+1		;13
		GOTO $+1		;15
		
		BSF POORT7								;1
	    BTFSC ((LOW TBL_BINAIR)+5),4,BANKED		;3
		GOTO TBL_SPR1							;5
		GOTO $+1									;    5
		GOTO TBL_SPR2
TBL_SPR1		
		BTFSS KNIPPERBIT						;7
		GOTO TBL_SPR3	;GEEN LED
TBL_SPR2
		DECFSZ WREG,W
		GOTO $-1
				
		BCF POORT7		;1
		GOTO $+1		;3
		GOTO $+1		;5
		GOTO $+1		;7
		GOTO $+1		;9
		GOTO $+1		;11
		GOTO $+1		;13
		GOTO $+1		;15		
		BSF POORT7
TBL_SPR3

		BSF INTCON,7


ENDIF

		
UL_SPR10
	    MOVLB HIGH TBL_FUNCTIE

	
    	MACRO_FADE_LED 1
    	MACRO_FADE_LED 2
    	MACRO_FADE_LED 3
    	MACRO_FADE_LED 4
    	MACRO_FADE_LED 5
    	MACRO_FADE_LED 6
    	MACRO_FADE_LED 7
    	MACRO_FADE_LED 8
    	MACRO_FADE_LED 9
    	MACRO_FADE_LED 10
       	MACRO_FADE_LED 11
	    MACRO_FADE_LED 12
    	MACRO_FADE_LED 13
	    MACRO_FADE_LED 14
    	MACRO_FADE_LED 15
    	MACRO_FADE_LED 16
    	MACRO_FADE_LED 17
    	MACRO_FADE_LED 18
    	MACRO_FADE_LED 19
    	MACRO_FADE_LED 20
    	MACRO_FADE_LED 21
    	MACRO_FADE_LED 22
    	MACRO_FADE_LED 23
    	MACRO_FADE_LED 24
    	MACRO_FADE_LED 25
    	MACRO_FADE_LED 26
    	MACRO_FADE_LED 27
    	MACRO_FADE_LED 28
    IF DEBUG == 0
    	MACRO_FADE_LED 29
    	MACRO_FADE_LED 30
    ENDIF

 
    	RETURN