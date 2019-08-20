
;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 15 juli 2019                                                     *
;*                                                                         *
;* Filenaam: LOCONET_IO_LED_V6p0.INC                                       *
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

    COMF WREG                                       ;15/7/2019 toegevoegd om flikkeren bij waarde 0xFF tegen te gaan
    BTFSC STATUS,Z                                  ;15/7/2019
    BRA $+8                                         ;15/7/2019
    COMF WREG                                       ;15/7/2019
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
    IF DEBUGGER == 0
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

		MOVF ((LOW TBL_CONST3)+(LED_NR-1)),W,BANKED
		RLNCF WREG,W
;		ADDLW 2
        MOVWF TABEL_GOTO

    	BTFSS ((LOW TBL_BINAIR)+(LED_NR-1)),2,BANKED 	;  JA: is gewenste LED stand MAX
    	BRA MFD_SPR20
		
        BCF STATUS,C                                	;      JA: verhoog VAR1+VAR2 met waarde CONST3
		CALL TABEL0
		ADDWF ((LOW TBL_VAR2)+(LED_NR-1)),F,BANKED
		BTFSC STATUS,C
		INCF ((LOW TBL_VAR1)+(LED_NR-1)),F,BANKED 
        BZ MFD_SPR15
		CALL TABEL256		
    	ADDWF ((LOW TBL_VAR1)+(LED_NR-1)),F,BANKED  

    	BC MFD_SPR15
    	MOVF ((LOW TBL_VAR1)+(LED_NR-1)),W,BANKED
    	CPFSLT ((LOW TBL_CONST2)+(LED_NR-1)),BANKED  	;          Is VAR1 >= MAX dan VAR1=MAX en einde Fade IN	naar MAX	
    	BRA MFD_END

MFD_SPR15
    	MOVFF (TBL_CONST2+(LED_NR-1)),(TBL_VAR1+(LED_NR-1))
		CLRF  ((LOW TBL_VAR2)+(LED_NR-1)),BANKED
    	BCF ((LOW TBL_BINAIR)+(LED_NR-1)),0,BANKED
    	BSF ((LOW TBL_BINAIR)+(LED_NR-1)),1,BANKED
	    BRA MFD_END

		
MFD_SPR20
        BCF STATUS,C                                	;      NEE: verlaag VAR1+VAR2 met waarde CONST3                     
		CALL TABEL0
		SUBWF ((LOW TBL_VAR2)+(LED_NR-1)),F,BANKED
        BCF COPY_C,0
        BTFSC STATUS,C
        BSF COPY_C,0	
		CALL TABEL256
		BTFSS COPY_C,0
		INCFSZ WREG,F
    	SUBWF ((LOW TBL_VAR1)+(LED_NR-1)),F,BANKED	    ;          Is VAR1 <= MIN dan VAR1=MIN en einde Fade OUT naar MIN

    	BNC MFD_SPR30
    	MOVF ((LOW TBL_VAR1)+(LED_NR-1)),W,BANKED
    	CPFSGT ((LOW TBL_CONST1)+(LED_NR-1)),BANKED
    	BRA MFD_END

MFD_SPR30
    	MOVFF (TBL_CONST1+(LED_NR-1)),(TBL_VAR1+(LED_NR-1))
		CLRF  ((LOW TBL_VAR2)+(LED_NR-1)),BANKED
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
    IF DEBUGGER == 0
    	MACRO_FADE_LED 29
    	MACRO_FADE_LED 30
    ENDIF
		
    	RETURN
		
    ORG ($ -($%256) +256)

TABEL256
        MOVF PCL,W
        MOVF TABEL_GOTO,W
	    ADDWF PCL,F
	    RETLW .42
	    RETLW .4
        RETLW .2
        RETLW .1
        RETLW .1
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .0
        RETLW .42
        RETLW .42
        RETLW .21
        RETLW .14
        RETLW .10
        RETLW .8
        RETLW .7
        RETLW .6
        RETLW .5
        RETLW .4
        RETLW .4

    ORG ($ -($%256) +256)
TABEL0
        MOVF PCL,W
        MOVF TABEL_GOTO,w
	    ADDWF PCL,F
        RETLW .170		
        RETLW .68
        RETLW .34
        RETLW .108
        RETLW .17
        RETLW .218
        RETLW .182
        RETLW .156
        RETLW .136
        RETLW .121
        RETLW .109
        RETLW .99
        RETLW .91
        RETLW .84
        RETLW .78
        RETLW .72
        RETLW .68
        RETLW .64
        RETLW .60
        RETLW .57
        RETLW .54
        RETLW .52
        RETLW .49
        RETLW .47
        RETLW .45
        RETLW .43
        RETLW .42
        RETLW .40
        RETLW .39
        RETLW .37
        RETLW .36
        RETLW .35
        RETLW .34
        RETLW .33
        RETLW .32
        RETLW .31
        RETLW .30
        RETLW .29
        RETLW .28
        RETLW .28
        RETLW .27
        RETLW .26
        RETLW .26
        RETLW .25
        RETLW .24
        RETLW .24
        RETLW .23
        RETLW .23
        RETLW .22
        RETLW .22
        RETLW .21
        RETLW .21
        RETLW .21
        RETLW .20
        RETLW .20
        RETLW .19
        RETLW .19
        RETLW .19
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .18
        RETLW .170
        RETLW .170
        RETLW .85
        RETLW .56
        RETLW .170
        RETLW .136
        RETLW .28
        RETLW .24
        RETLW .85
        RETLW .189
        RETLW .68
