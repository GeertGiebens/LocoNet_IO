
;***************************************************************************
;* LocoNet In/out module met PIC 18F4620/18F4520 microcontroller           *
;*  30 in-uitgangen met keuze uit:                                         *
;*  - Servo uitgangen                                                      *
;*  - logische uitgangen (5V Relais modules)                               *
;*  - logische uitgangen puls (5V Relais modules voor wisselspoel)         *
;*  - LED uitgangen (Fade in Fade out)                                     *
;*  - bezetmelders (massa)                                                 *
;*  - schakelaar; drukknop; enz... (met debouncing)                        *
;*  - mogelijke toekomstige uitbreidingen                                  *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620/18F4520 microcontroller  *
;*                                                                         *
;* Datum: 12 feb 2018                       Versie LOCONET IO V1.2         *
;*                                                                         *
;* Filenaam: LOCONET_IO_V1p2.ASM                                           *
;*     	                                                                   *
;* DISCLAIMER: LocoNet is een Copyrighted product van Digitrax Inc.        *
;*             De software en hardware mag enkel gebruikt worden    	   *
;*             voor persoonlijk gebruik en op risico van de                *
;*             gebruiker zelf. De auteur kan geen garantie                 *
;*             bieden op de correcte werking van deze software.     	   *
;*     	                                                                   *
;*     	                                                                   *
;* Aanpassingen versie 1.1:                                                *
;*  -19/12/16: Optie LED1;LED2;LED3 en LED4 komt te vervallen              *
;*  -19/12/16: Optie ingang ON/OFF met mogelijkheid voor meerdere adressen *
;*  -20/12/16: Initiatie Servo's (terug juist plaatsen na opstart)         *
;*  -21/12/16: Code opgesplitst in diverse files                           *
;*  -06/01/17: Al de gebruikte variabelen in 1 INC file verzameld    	   *
;*  -10/05/17: Verder opsplitsen codes in diverse files                    *
;*  -17/07/17: Testen met inwendige comparator om externe te vervangen     *
;*     	                                                                   *
;* Aanpassingen versie 1.2:                                                *
;*  -12/02/18: aanpassingen voor gebruik ook met 18F4520    	           *
;*     	                                                                   *
;*                         _____________________                           *
;*     (Vpp)   PROGRAM    | 1  RE3 *  *  RB7 40 |      poort 30 (PGD)      *
;*      Comperator Vin+   | 2  RA0  **   RB6 39 |      poort 29 (PGC)      *
;*             poort  1   | 3  RA1       RB5 38 |      poort 28            *
;*             poort  2   | 4  RA2       RB4 37 |      poort 27            *
;*      Comperator Vin-   | 5  RA3   I   RB3 36 |      poort 26            *
;*      Comperator  Out   | 6  RA4   C   RB2 35 |      poort 25            *
;*             poort  3   | 7  RA5       RB1 34 |      poort 24            *
;*             poort  4   | 8  RE0   P   RB0 33 |      poort 23            *
;*             poort  5   | 9  RE1   I       32 |     +5V	               *
;*             poort  6   | 10 RE2   C       31 |     Massa	               *
;*                   +5V  | 11           RD7 30 |      poort 22            *
;*                 Massa  | 12       1   RD6 29 |      poort 21            *
;*             poort  7   | 13 RA7   8   RD5 28 |      poort 20            *
;*             poort  8   | 14 RA6   F   RD4 27 |      poort 19            *
;*             poort  9   | 15 RC0   4   RC7 26 |     LN_RECEIVER          *
;*             poort 10   | 16 RC1   6   RC6 25 |     LN_TRANSMITTER       *
;*             poort 11   | 17 RC2   2   RC5 24 |      poort 18            *
;*             poort 12   | 18 RC3   0   RC4 23 |      poort 17            *
;*             poort 13   | 19 RD0       RD3 22 |      poort 16            *
;*             poort 14   | 20 RD1       RD2 21 |      poort 15            *
;*                         _____________________                           *
;*                                                                         *								    
;***************************************************************************

	   LIST P=18F4620 
	   #include <P18F4620.INC> 
 
;	   LIST P=18F4520 
;	   #include <P18F4520.INC> 
 


     CONFIG OSC =  INTIO67
     CONFIG PWRT = ON, BOREN = OFF
     CONFIG WDT = OFF
     CONFIG MCLRE = OFF,  LPT1OSC = OFF, PBADEN = OFF
     CONFIG STVREN = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF
     CONFIG CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
     CONFIG CPB = OFF, CPD = OFF

    RADIX DEC

    #DEFINE DEBUG 1
	#DEFINE PIC18F4620 1

    #include LOCONET_IO_EEPROM_V1p2.INC

	#include LOCONET_IO_VARIABELEN_V1p2.INC



	
;******************************************************************************
;*******  MAIN PROGRAM  *******************************************************
;******************************************************************************

    ORG 00000h
 	GOTO MAIN						;LOCONET_IO_V1p2.ASM

    ORG 00008h
	GOTO High_Priority_Interrupt	;LOCONET_IO_LED.INC

    ORG 00018h
	GOTO Low_Priority_Interrupt		;LOCONET_18F4620_V1p0.INC

	
;********************************************************************************

    #include LOCONET_18F4X20_V1p2.INC

    #include LOCONET_IO_PROCEDURES_V1p2.INC

    #include LOCONET_IO_SERVO.INC

    #include LOCONET_DECODEER_V1p2.INC

    #include LOCONET_IO_LED.INC

    #include LOCONET_IO_INGANG_V1p2.INC

    #include LOCONET_IO_SPOEL.INC

    #include LOCONET_IO_RELAIS.INC


;**********************************************************
;*******  INIT MAIN PROGRAM  ******************************
;**********************************************************

INIT_MAIN

;INIT_OSCILATOR

	    BSF OSCCON,IRCF0        ;zet oscilator op 32MHz (in deze volgorde bit's zetten!!)
	    BSF OSCCON,IRCF1  
	    BSF OSCTUNE,PLLEN  	
		
;INIT Timer 3 op 60Hz --> interrupt TMR3IF

	    CLRF TMR3L  
    	CLRF TMR3H   
    	MOVLW b'00010001'
    	MOVWF T3CON  

;INIT Timer 0 om pulsen op te wekken van 1ms tot 255ms

        MOVLW b'00000100'   ;prescaler 1/32
        MOVWF T0CON  

        CLRF POSITIE_TELLER_IN     
        CLRF POSITIE_TELLER_OUT
        CLRF VERSCHIL_TELLER

        MOVLW 0Fh                   ;geen analoge ingangen
        MOVWF ADCON1

        RETURN


;**********************************************************
;*******  MACRO INIT POORT EN TABEL VARIABELEN   **********
;**********************************************************

MACRO_INIT MACRO POORTNR ,POORT, POORTBIT
     
        LOCAL MI_END,MI_SPR10, MI_SPR20,MI_SPR30, MI_SPR25, MI_SPR35, MI_SPR40, MI_SPR50, MI_SPR32,MI_SPR45

        MOVF (LOW TBL_FUNCTIE + (POORTNR-1)),F,BANKED       ;is FUNCTIE = NOP dan poort = ingang
        BNZ MI_SPR10 
        BSF (POORT+12h),POORTBIT                            ;zet TRIS BIT register op ingang
        BRA MI_END    

MI_SPR10
        BTFSS (LOW TBL_FUNCTIE + (POORTNR-1)),0,BANKED     ;is FUNCTIE,BIT0 = 1 dan poort = ingang
        BRA MI_SPR20

        BSF (POORT+12h),POORTBIT                            ;zet TRIS BIT register op ingang
        CLRF (LOW TBL_VAR1 + (POORTNR-1)),BANKED           ;startwaarde antidender=00
        CLRF (LOW TBL_BINAIR +(POORTNR-1)),BANKED          ;startwaarde logische toestand ingang ='0'
        BRA MI_END
        
MI_SPR20
        BCF (POORT+12h),POORTBIT                              ;zet TRIS BIT register op uitgang

        BTFSS ((LOW TBL_FUNCTIE) +(POORTNR-1)),7,BANKED      ;********IS FUNCTIE SERVO?********
        BRA MI_SPR30
        BCF POORT,POORTBIT                                   ;  JA: zet logische toestand uitgang = '0'
;        BCF (LOW TBL_BINAIR +(POORTNR-1)),0,BANKED
;        BCF (LOW TBL_BINAIR +(POORTNR-1)),3,BANKED
        BSF (LOW TBL_BINAIR +(POORTNR-1)),5,BANKED           ;     Ook initiatie
        BTFSC (LOW TBL_BINAIR +(POORTNR-1)),1,BANKED         ;      afhankelijk van huidige stand servo zet startwaarde servotijd op MIN of MAX
        BRA MI_SPR25
;        BCF (LOW TBL_BINAIR +(POORTNR-1)),2,BANKED
        MOVF (LOW TBL_CONST1 +(POORTNR-1)),W,BANKED
        MOVWF (LOW TBL_VAR1 + (POORTNR-1)),BANKED
        BRA MI_END
MI_SPR25
;        BSF (LOW TBL_BINAIR +(POORTNR-1)),2,BANKED
        MOVF (LOW TBL_CONST2 +(POORTNR-1)),W,BANKED
        MOVWF (LOW TBL_VAR1 + (POORTNR-1)),BANKED
        BRA MI_END

MI_SPR30
        BTFSS ((LOW TBL_FUNCTIE) +(POORTNR-1)),6,BANKED      ;********IS FUNCTIE LED?********
        BRA MI_SPR40
        BCF POORT,POORTBIT                                   ;  JA: zet logische toestand uitgang = '0'
    	BCF ((LOW TBL_BINAIR) + (POORTNR-1)),0,BANKED
        BCF ((LOW TBL_BINAIR) +(POORTNR-1)),3,BANKED
        BTFSS ((LOW TBL_FUNCTIE) + (POORTNR-1)),1,BANKED      ;      als bit1 van FUNCTIE = '1' dan is functie LED 'knipper' 
        BRA MI_SPR32
        BSF ((LOW TBL_BINAIR) +(POORTNR-1)),3,BANKED
MI_SPR32
        BTFSC ((LOW TBL_BINAIR) +(POORTNR-1)),1,BANKED         ;      afhankelijk van huidige stand LED zet startwaarde LEDDIM op MIN of MAX
        BRA MI_SPR35
        MOVF ((LOW TBL_CONST1) +(POORTNR-1)),W,BANKED
        MOVWF ((LOW TBL_VAR1) + (POORTNR-1)),BANKED
        BRA MI_END
MI_SPR35
        BSF ((LOW TBL_BINAIR) +(POORTNR-1)),2,BANKED
        MOVF ((LOW TBL_CONST2) +(POORTNR-1)),W,BANKED
        MOVWF ((LOW TBL_VAR1) + (POORTNR-1)),BANKED
        BRA MI_END

MI_SPR40
        BTFSS ((LOW TBL_FUNCTIE) +(POORTNR-1)),4,BANKED      ;********IS FUNCTIE SPOEL (COIL)
        BRA MI_SPR45
        BCF POORT,POORTBIT                                   ;  JA: zet logische toestand uitgang = '0'
        BRA MI_END    

MI_SPR45
        BTFSS ((LOW TBL_FUNCTIE) +(POORTNR-1)),3,BANKED      ;********IS FUNCTIE RELAIS
        BRA MI_SPR50
        BCF POORT,POORTBIT                                   ;  JA: zet logische toestand uitgang = '0'

        MOVF ((LOW TBL_CONST2)+ (POORTNR-1)),W,BANKED       ;    als T2 niet 0 is dan reset BINAIR (voorkomt dat relais even aan gaat met T2)
        BZ MI_END
        CLRF ((LOW TBL_BINAIR)+ (POORTNR-1)),BANKED 
        BRA MI_END     

MI_SPR50                                                     ;*********FUNCTIE IS NOP (niks dus!)
        BSF (POORT+12h),POORTBIT                             ;zet TRIS BIT register op ingang voor de zekerheid

MI_END
        NOP
        ENDM
		
;**********************************************************

;INIT POORTEN en Tabel Variabelen

INIT_POORTEN 

        MOVLB HIGH TBL_FUNCTIE

        MACRO_INIT 1,POORT1         ;INIT POORT 1 en bijbehorende tabel variabelen
        MACRO_INIT 2,POORT2    
        MACRO_INIT 3,POORT3
        MACRO_INIT 4,POORT4
        MACRO_INIT 5,POORT5
        MACRO_INIT 6,POORT6
        MACRO_INIT 7,POORT7
        MACRO_INIT 8,POORT8
        MACRO_INIT 9,POORT9
        MACRO_INIT 10,POORT10
        MACRO_INIT 11,POORT11
        MACRO_INIT 12,POORT12
        MACRO_INIT 13,POORT13
        MACRO_INIT 14,POORT14
        MACRO_INIT 15,POORT15
        MACRO_INIT 16,POORT16
        MACRO_INIT 17,POORT17
        MACRO_INIT 18,POORT18
        MACRO_INIT 19,POORT19
        MACRO_INIT 20,POORT20
        MACRO_INIT 21,POORT21
        MACRO_INIT 22,POORT22
        MACRO_INIT 23,POORT23
        MACRO_INIT 24,POORT24
        MACRO_INIT 25,POORT25
        MACRO_INIT 26,POORT26
        MACRO_INIT 27,POORT27
        MACRO_INIT 28,POORT28

    IF DEBUG == 1   ;BIJ DEBUGGER WORDEN POORT 29 EN 30 NIET GEBRUIKT!
        MACRO_INIT 29,POORT29
        MACRO_INIT 30,POORT30
    ENDIF
        RETURN
		
;**********************************************************

;COPY DATA UIT EEPROM

INIT_EEPROM_TO_RAM

        BCF EECON1, EEPGD       ; Point to DATA memory
        BCF EECON1, CFGS        ; Access EEPROM

        CLRF EEADRH 
        CLRF EEADR     
        MOVLW HIGH TBL_FUNCTIE  ;selecteer bank waar de tabel FUNCTIE staat
        MOVWF FSR2H  
        MOVLW LOW TBL_FUNCTIE 
        MOVWF FSR2L  
        CLRF HULPTEL  

IM_SPR50
        BSF EECON1, RD            ; EEPROM Read
        MOVF EEDATA, W            ; W = EEDATA
        MOVWF POSTINC2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA IM_SPR50

	IF PIC18F4620 == 1		
        INCF EEADRH,F    
IM_SPR55
        BSF EECON1, RD            ; EEPROM Read
        MOVF EEDATA, W            ; W = EEDATA
        MOVWF POSTINC2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA IM_SPR55

        INCF EEADRH,F    
IM_SPR60
        BSF EECON1, RD            ; EEPROM Read
        MOVF EEDATA, W            ; W = EEDATA
        MOVWF POSTINC2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA IM_SPR60

        INCF EEADRH,F    
IM_SPR65
        BSF EECON1, RD            ; EEPROM Read
        MOVF EEDATA, W            ; W = EEDATA
        MOVWF POSTINC2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA IM_SPR65
	ENDIF
	
	;data van 0x2A0-0x2DF --> 0x300-0x33F 
	
	    MOVLW 64
	    MOVWF HULPTEL
	    MOVLW HIGH TBL_VAR1  ;selecteer bank waar de tabel VAR1
        MOVWF FSR2H  
        MOVLW LOW TBL_VAR1 
        MOVWF FSR2L
		MOVLW HIGH TBL_ADR1L  ;selecteer bank waar de tabel ADR1L
        MOVWF FSR0H  
        MOVLW LOW TBL_ADR1L 
        MOVWF FSR0L
IM_SPR70
		MOVF POSTINC2,W
		MOVWF POSTINC0	
	    DECFSZ HULPTEL,F
		BRA IM_SPR70
		
		MOVLB HIGH TBL_INSTELLINGEN
        MOVF LOW TBL_INSTELLINGEN,w
        MOVWF DEVICENUMMER

		RETURN


;**********************************************************
;*******  MAIN   ******************************************
;**********************************************************

MAIN
        CALL INIT_MAIN                      ;LOCONET_IO_V1p2.ASM
		CALL INIT_EEPROM_TO_RAM				;LOCONET_IO_V1p2.ASM
        CALL INIT_POORTEN                   ;LOCONET_IO_V1p2.ASM
        CALL INIT_SERVO                     ;LOCONET_IO_SERVO.INC
        CALL INIT_LOCONET                   ;LOCONET_18F4X20_V1p2.INC     

LUS
	    BTFSS PIR2,TMR3IF  	                ;Timer3 overflow?
	    BRA VOLGENDE
		
	    BCF PIR2,TMR3IF                 	;YES:  (wordt elke 16,6ms uitgevoerd 60Hz)	
	    CALL RESET_SERVO                    ;  LOCONET_IO_SERVO.INC
	    CALL UPDATE_LED                     ;  LOCONET_IO_LED.INC
        CALL RELAIS                         ;  LOCONET_IO_RELAIS.INC

VOLGENDE
        CALL TEST_INGANG                    ;LOCONET_IO_INGANG_V1p2.INC
        CALL TEST_NIEUWE_TE_VERZENDEN_DATA  ;LOCONET_IO_PROCEDURES_V1p2.INC
        CALL DECODEER_LOCONET_DATA          ;LOCONET_DECODEER_V1p2.INC
	    CALL SERVO                          ;LOCONET_IO_SERVO.INC
	    CALL LED                            ;LOCONET_IO_LED.INC
        CALL SPOEL                          ;LOCONET_IO_SPOEL.INC
        GOTO LUS

        END 
	