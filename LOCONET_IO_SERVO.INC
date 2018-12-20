
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
;* Datum: 23 JUL 2017                      Versie LOCONET IO V1.1           *
;*                                                                         *
;* Filenaam: LOCONET_IO_SERVO.INC                                          *
;*                                                                         *
;***************************************************************************


	#DEFINE MAXIMUM_SERVO 	5		;maximum servo's die tergelijkertijd mogen bewegen om de 16,6ms


;**********************************************************
;*******  SERVO: High Priority Interrupt  *****************
;**********************************************************


;deze intterupt zorgt er voor dat een servo puls op een uitgang LOW wordt
;deze timing moet juist zijn, en staat daarom in de High priorty int
;
;  byte: poortnummer 1-32 --> geeft weer welke servo uitgang actief is
;  bit: T2CON,TMR2ON = 1  -> geeft aan dat er een servopuls op een uitgang staat)


High_Priority_Interrupt

    ORG $-($%256)+256          ;Zet eertvolgende adres op nnn00h (nodig bij ADDWF PCL die enkel de laagste byte van adres kan beïnvloeden)
	
	MOVF PCL,W			;update PCLATU PCLATH
	BCF PIR1,TMR2IF		        ;clear flag INT Timer2
	BCF T2CON,TMR2ON		    ;stop timer2 

	RLNCF POORTNUMMER,W
	RLNCF WREG,W		    	; POORTRNUMMER x 4 --> WREG 
	ADDWF PCL,F  		    	; Program counter + WREG = sprongadres

	BCF POORT1  
	RETFIE FAST

	BCF POORT2  
	RETFIE FAST

	BCF POORT3  
	RETFIE FAST

	BCF POORT4  
	RETFIE FAST

	BCF POORT5  
	RETFIE FAST

	BCF POORT6  
	RETFIE FAST

	BCF POORT7  
	RETFIE FAST

	BCF POORT8  
	RETFIE FAST

	BCF POORT9  
	RETFIE FAST

	BCF POORT10  
	RETFIE FAST

	BCF POORT11  
	RETFIE FAST

	BCF POORT12  
	RETFIE FAST

	BCF POORT13  
	RETFIE FAST

	BCF POORT14  
	RETFIE FAST

	BCF POORT15  
	RETFIE FAST

	BCF POORT16  
	RETFIE FAST

	BCF POORT17  
	RETFIE FAST

	BCF POORT18  
	RETFIE FAST

	BCF POORT19  
	RETFIE FAST

	BCF POORT20  
	RETFIE FAST

	BCF POORT21  
	RETFIE FAST

	BCF POORT22  
	RETFIE FAST

	BCF POORT23  
	RETFIE FAST

	BCF POORT24  
	RETFIE FAST

	BCF POORT25  
	RETFIE FAST

	BCF POORT26  
	RETFIE FAST

	BCF POORT27  
	RETFIE FAST

	BCF POORT28  
	RETFIE FAST

	BCF POORT29  
	RETFIE FAST

	BCF POORT30  
	RETFIE FAST





;***********************************************************
;*******  SERVO: RESET voor nieuwe pulsen elke 16,6ms  *****
;***********************************************************

MACRO_RESET_SERVO MACRO MRS_NR
    
        BTFSC ((LOW TBL_FUNCTIE)+(MRS_NR-1)),7,BANKED    ;als Functie=SERVO dan reset BINAIR Bit0
        BCF ((LOW TBL_BINAIR)+(MRS_NR-1)),3,BANKED
        ENDM 


RESET_SERVO
        MOVLB HIGH TBL_FUNCTIE

        MACRO_RESET_SERVO 1
        MACRO_RESET_SERVO 2
        MACRO_RESET_SERVO 3
        MACRO_RESET_SERVO 4
        MACRO_RESET_SERVO 5
        MACRO_RESET_SERVO 6
        MACRO_RESET_SERVO 7
        MACRO_RESET_SERVO 8
        MACRO_RESET_SERVO 9
        MACRO_RESET_SERVO 10
        MACRO_RESET_SERVO 11
        MACRO_RESET_SERVO 12
        MACRO_RESET_SERVO 13
        MACRO_RESET_SERVO 14
        MACRO_RESET_SERVO 15
        MACRO_RESET_SERVO 16
        MACRO_RESET_SERVO 17
        MACRO_RESET_SERVO 18
        MACRO_RESET_SERVO 19
        MACRO_RESET_SERVO 20
        MACRO_RESET_SERVO 21
        MACRO_RESET_SERVO 22
        MACRO_RESET_SERVO 23
        MACRO_RESET_SERVO 24
        MACRO_RESET_SERVO 25
        MACRO_RESET_SERVO 26
        MACRO_RESET_SERVO 27
        MACRO_RESET_SERVO 28
    IF DEBUGGER == 0
        MACRO_RESET_SERVO 29
        MACRO_RESET_SERVO 30
    ENDIF
        NOP
        RETURN


;***********************************************************
;*******  SERVO: INIT  *************************************
;***********************************************************

;INIT Timer2 en Timer2 Interrupt

;basis = 10µs (bij 32MHz klokfreqentie)
;PR2 range=50-250 komt overeen met 500µs tot 2500µs
;start puls door TMR2 de wissen en TMR2ON te setten

INIT_SERVO

    MOVLW  b'00100010'
            ; 0100    =postscaler 1/5	
            ;      10 =prescaler 1/16 
    MOVWF T2CON  

	BSF PIE1,TMR2IE  
	BSF RCON,IPEN		;enable High Priorty
    CLRF IPR1           
    CLRF IPR2  
    CLRF INTCON3  
    CLRF INTCON2        ;+interne pull-ups POORTB 
	BSF IPR1,TMR2IP   	;enkel Timer2 heeft hoogste prioriteit	
	BSF INTCON,GIEH

  

;INIT SERVO Variabelen


   CLRF SERVO_TEL      ;startwaarde servo teller 

	RETURN


;**********************************************************
;*******  SERVO: MACRO_SERVO ******************************
;**********************************************************

;Waarden van BINAIR b'76543210'
;                       ||| ||
;                       ||| |b=1 Servo beweegt
;                       ||| b=0  Servo staat in min stand    b=1 Servo staat in max stand
;                       ||b=1    Servo uitgang heeft puls uitgestuurd tijdens 16,6ms (wordt gereset elke 16,6ms)
;                       |b=0     Gewenste stand servo min    b=1 Gewenste stand servo max
;                       b=0      Initiatie uitgevoerd        b=1 Initiatie moet nog uitgevoert worden (initiatie is bij onder spanning komen Servo deze terug juist plaatsen)  

;CONST1 = MIN waarde SERVO waarde tussen 50-X1 ms
;CONST2 = MAX waarde SERVO waarde tussen X2-250 ms   waarbij X2>X1
;CONST3 = VERTRAGING SERVO 1-255 waarbij 255 SNEL is
;VAR1 = huidige pulstijd SERVO tussen 50-250 ms
;VAR2 = vertragings teller die bijhoud hoe snel servo beweegt.  VAR2 = VAR2+CONST3; bij Carry overgang pulstijd +1 of -1 


MACRO_SERVO MACRO SERVONR, POORT, POORTBIT

	LOCAL MS_END, MS_SPR5, MS_SPR7, MS_SPR10, MS_SPR20, MS_SPR25, MS_SPR30, MS_SPR40


        BTFSS ((LOW TBL_FUNCTIE)+(SERVONR-1)),7,BANKED     ;is poortfunctie = SERVO?
        BRA MS_END                                         ;  NEE: dan volgende Poort

        BTFSC ((LOW TBL_BINAIR)+(SERVONR-1)),3,BANKED      ;heeft servo uitgang al een puls uitgestuurd de laatste 20ms?
        BRA MS_END                                         ;  JA: dan volgende Poort
        
        BTFSS ((LOW TBL_BINAIR)+(SERVONR-1)),0,BANKED      ;is SERVO aan het bewegen?
        BRA MS_SPR10

        BTFSS ((LOW TBL_BINAIR)+(SERVONR-1)),5,BANKED      ;  JA1: Is Servo bezig met Initiatie?
        BRA MS_SPR5
        DECFSZ ((LOW TBL_VAR2)+(SERVONR-1)),F,BANKED       ;      Is Initiatie voorbij? 
        BRA MS_SPR40                                       ;        NEE: dan stuur nieuwe puls
        BCF ((LOW TBL_BINAIR)+(SERVONR-1)),5,BANKED        ;        JA: einde Initiatie 
        BCF ((LOW TBL_BINAIR)+(SERVONR-1)),0,BANKED
        DECF SERVO_TEL,F 
        BRA MS_END  
                                                
MS_SPR5        
        BCF STATUS,C  
        MOVF ((LOW TBL_CONST3)+(SERVONR-1)),W,BANKED       ;  JA2: verhoog vertragings teller
        ADDWF ((LOW TBL_VAR2)+(SERVONR-1)),F,BANKED
        BNC MS_SPR40                                       ;      is er een Carry overgang?
        
        BTFSS ((LOW TBL_BINAIR)+(SERVONR-1)),4,BANKED      ;        JA: beweegt servo naar MAX stand
        BRA MS_SPR30
        INCF ((LOW TBL_VAR1)+(SERVONR-1)),F,BANKED         ;            JA: verhoog pulslengte
        MOVF ((LOW TBL_VAR1)+(SERVONR-1)),W,BANKED         ;                is pulslengte = MAX servo waarde
        CPFSEQ ((LOW TBL_CONST2)+(SERVONR-1)),BANKED       ;                 JA: dan SERVO staat in juiste stand
        BRA MS_SPR40   
        BCF ((LOW TBL_BINAIR)+(SERVONR-1)),0,BANKED
        BSF ((LOW TBL_BINAIR)+(SERVONR-1)),1,BANKED
        DECF SERVO_TEL,F  
        BRA MS_SPR40

MS_SPR30
        DECF ((LOW TBL_VAR1)+(SERVONR-1)),F,BANKED         ;            NEE: verlaag pulslengte
        MOVF ((LOW TBL_VAR1)+(SERVONR-1)),W,BANKED          ;                is pulslengte = MIN servo waarde
        CPFSEQ ((LOW TBL_CONST1)+(SERVONR-1)),BANKED       ;                 JA: dan SERVO staat in juiste stand
        BRA MS_SPR40   
        BCF ((LOW TBL_BINAIR)+(SERVONR-1)),0,BANKED
        BCF ((LOW TBL_BINAIR)+(SERVONR-1)),1,BANKED
        DECF SERVO_TEL,F  
        BRA MS_SPR40

MS_SPR10
        MOVLW MAXIMUM_SERVO
        CPFSLT SERVO_TEL                                   ;  NEE: (servo beweegt niet) Zijn er al 5 servo's aan het bewegen
        BRA MS_END                                         ;      JA: dan volgende poort

        BTFSS ((LOW TBL_BINAIR)+(SERVONR-1)),5,BANKED      ;      Moet Servo een Initiatie krijgen?
        BRA MS_SPR7 
        BSF ((LOW TBL_BINAIR)+(SERVONR-1)),0,BANKED        ;        JA: geef aan dat servo aan het bewegen is
        INCF SERVO_TEL,F                                   ;            verhoog servo teller
        MOVLW 60                                           ;            start VAR2 met 60 = 1seconde pulsen uitsturen
        MOVWF ((LOW TBL_VAR2)+(SERVONR-1)),BANKED 
        BRA MS_SPR40                                       ;            en stuur eerste puls uit                                     

MS_SPR7              
        BTFSC ((LOW TBL_BINAIR)+(SERVONR-1)),4,BANKED      ;is de gewenste stand MIN en staat servo op MIN dan volgende poort  
        BRA MS_SPR20
        BTFSS ((LOW TBL_BINAIR)+(SERVONR-1)),1,BANKED
        BRA MS_END
        BRA MS_SPR25

MS_SPR20
        BTFSC ((LOW TBL_BINAIR)+(SERVONR-1)),1,BANKED      ; is de gewenste stand MAX en staat servo MAX dan volgende poort
        BRA MS_END
MS_SPR25                                                   ; INDIEN Anders doe:
        BSF ((LOW TBL_BINAIR)+(SERVONR-1)),0,BANKED        ;   geef aan dat servo aan het bewegen is
        INCF SERVO_TEL,F                                   ;   verhoog servo teller
        CLRF ((LOW TBL_VAR2)+(SERVONR-1)),BANKED           ;    reset vertragingsteller

MS_SPR40
        MOVF  ((LOW TBL_VAR1)+(SERVONR-1)),W,BANKED       ;  START Timer2 met waarde pulslengte servo en zet uitgangspoort
        MOVWF PR2  
        MOVLW (SERVONR-1)
        MOVWF POORTNUMMER  
        BSF ((LOW TBL_BINAIR)+(SERVONR-1)),3,BANKED
        BCF INTCON,GIEL                                     ;  (even geen INT)
        BSF POORT,POORTBIT   
        BSF T2CON,TMR2ON    
        BSF INTCON,GIEL  
        RETURN
     
MS_END
	NOP	
	ENDM


;**********************************************************
;*******  SERVO: HOOFPROGRAM ******************************
;**********************************************************

SERVO
        BTFSC T2CON,TMR2ON  	;is er een servopuls op één van de uitgangen?  dan doe niets
        RETURN

        MOVLB HIGH TBL_FUNCTIE  ;selecteer bank waar de tabellen staan


    	MACRO_SERVO 1, POORT1
    	MACRO_SERVO 2, POORT2
    	MACRO_SERVO 3, POORT3
    	MACRO_SERVO 4, POORT4
    	MACRO_SERVO 5, POORT5
    	MACRO_SERVO 6, POORT6
    	MACRO_SERVO 7, POORT7
    	MACRO_SERVO 8, POORT8
    	MACRO_SERVO 9, POORT9
    	MACRO_SERVO 10, POORT10
     	MACRO_SERVO 11, POORT11
    	MACRO_SERVO 12, POORT12
    	MACRO_SERVO 13, POORT13
    	MACRO_SERVO 14, POORT14
    	MACRO_SERVO 15, POORT15
    	MACRO_SERVO 16, POORT16
    	MACRO_SERVO 17, POORT17
    	MACRO_SERVO 18, POORT18
    	MACRO_SERVO 19, POORT19
    	MACRO_SERVO 20, POORT20
    	MACRO_SERVO 21, POORT21
    	MACRO_SERVO 22, POORT22
    	MACRO_SERVO 23, POORT23
    	MACRO_SERVO 24, POORT24
    	MACRO_SERVO 25, POORT25
    	MACRO_SERVO 26, POORT26
    	MACRO_SERVO 27, POORT27
    	MACRO_SERVO 28, POORT28
    IF DEBUGGER == 0
    	MACRO_SERVO 29, POORT29
    	MACRO_SERVO 30, POORT30
    ENDIF
        RETURN

