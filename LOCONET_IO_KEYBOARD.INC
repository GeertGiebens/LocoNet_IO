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
;* Datum: 10 mei 2017                      Versie LOCONET IO V1.1          *
;*                                                                         *
;* Filenaam: LOCONET_IO_KEYBOARD.INC                                        *
;*                                                                         *
;***************************************************************************
;*                         _____________________                           *
;* (Vpp)       PROGRAM    | 1  RE3 *  *  RB7 40 |      poort 32 (PGD)      *
;*             DISPLAY    | 2  RA0  **   RB6 39 |      poort 31 (PGC)      *
;*               LCD_D7   | 3  RA1       RB5 38 |      poort 30            *
;*               LCD_D6   | 4  RA2       RB4 37 |      poort 29            *
;*               LCD_D5   | 5  RA3   I   RB3 36 |      poort 28            *
;*               LCD_D4   | 6  RA4   C   RB2 35 |      poort 27            *
;*               LCD_E    | 7  RA5       RB1 34 |      poort 26            *
;*               LCD_RS   | 8  RE0   P   RB0 33 |      poort 25            *
;*             KEY_OUT1   | 9  RE1   I       32 |     +5V	               *
;*             KEY_OUT2   | 10 RE2   C       31 |     Massa	               *
;*                   +5V  | 11           RD7 30 |      poort 22            *
;*                 Massa  | 12       1   RD6 29 |      poort 22            *
;*             KEY_OUT3   | 13 RA7   8   RD5 28 |      poort 24            *
;*             KEY_OUT4   | 14 RA6   F   RD4 27 |      poort 23            *
;*             KEY_OUT5   | 15 RC0   4   RC7 26 |     LN_RECEIVER          *
;*             KEY_IN1    | 16 RC1   6   RC6 25 |     LN_TRANSMITTER       *
;*             KEY_IN2    | 17 RC2   2   RC5 24 |      poort 20            *
;*             KEY_IN3    | 18 RC3   0   RC4 23 |      poort 19            *
;*             KEY_IN4    | 19 RD0       RD3 22 |      poort 18            *
;*                        | 20 RD1       RD2 21 |      poort 17            *
;*                         _____________________                           *
;*                                                                         *								    
;***************************************************************************

    #DEFINE KEY_OUT1                LATE,1
    #DEFINE KEY_OUT2                LATE,2
    #DEFINE KEY_OUT3                LATA,7
    #DEFINE KEY_OUT4                LATA,6
    #DEFINE KEY_OUT5                LATC,0
    #DEFINE KEY_IN1                 PORTC,1
    #DEFINE KEY_IN2                 PORTC,2
    #DEFINE KEY_IN3                 PORTC,3
    #DEFINE KEY_IN4                 PORTD,0

	
;********************************************************************
;*      KEYBOARD INIT
;********************************************************************

INIT_KEYBOARD

	BTFSS DISPLAY_ACTIEF
	RETURN

    BCF TRISE,1
    BCF TRISE,2
    BCF TRISA,7
    BCF TRISA,6
    BCF TRISC,0
    BSF TRISC,1
    BSF TRISC,2
    BSF TRISC,3
    BSF TRISD,0

	BCF KEY_OUT1
	BCF KEY_OUT2
	BCF KEY_OUT3
	BCF KEY_OUT4
	BCF KEY_OUT5

    RETURN

	
;********************************************************************
;*      CHECK KEYBOARD   
;********************************************************************

;Elke 16,66ms (60 keer per seconde)
;indien 2x achter toets LOW dan is deze ingedrukt 
;indien 2x achter elkaar geen toets ingedrukt dan volgende toets is een nieuwe.


; Alle poorten laag, check dan alle ingangen één voor één.
;   is er één ingang laag, dan zet één uitgang laag, check en zo verder.
;   anders return
; Toetsenvolgorde:
;
;    1  2  3  4
;    5  6  7  8
;    9  10 11 12
;    13 14 15 14
;    17 18 19 20
;
;Indien ingedrukt dan waarde PRESSED_KEY = 1...20 


CHECK_KEYBOARD
    BTFSS  DISPLAY_ACTIEF
    RETURN

	BTFSC KEY_IN1
	BRA KOLOM2
	MOVLW 1
	BRA WELKE_LIJN1

KOLOM2
 	BTFSC KEY_IN2
	BRA KOLOM3
	MOVLW 2
	BRA WELKE_LIJN2
KOLOM3  
	BTFSC KEY_IN3
	BRA KOLOM4
	MOVLW 3
	BRA WELKE_LIJN3
KOLOM4
	BTFSC KEY_IN4
	BRA GEEN_KEY_INGEDRUKT
	MOVLW 4

	BSF KEY_OUT2
	BSF KEY_OUT3
	BSF KEY_OUT4
	BSF KEY_OUT5
	BTFSC KEY_IN4
	BRA $+6
	ADDLW 0
	BRA CHK_KEY_END
	BSF KEY_OUT1
	BCF KEY_OUT2
	BTFSC KEY_IN4
	BRA $+6
	ADDLW 4
	BRA CHK_KEY_END
	BSF KEY_OUT2
	BCF KEY_OUT3
	BTFSC KEY_IN4
	BRA $+6
	ADDLW 8
	BRA CHK_KEY_END
	BSF KEY_OUT3
	BCF KEY_OUT4
	BTFSC KEY_IN4
	BRA $+6
	ADDLW 12
	BRA CHK_KEY_END
	ADDLW 16
	BRA CHK_KEY_END

WELKE_LIJN1
	
	BSF KEY_OUT2
	BSF KEY_OUT3
	BSF KEY_OUT4
	BSF KEY_OUT5
	BTFSC KEY_IN1
	BRA $+6
	ADDLW 0
	BRA CHK_KEY_END
	BSF KEY_OUT1
	BCF KEY_OUT2
	BTFSC KEY_IN1
	BRA $+6
	ADDLW 4
	BRA CHK_KEY_END
	BSF KEY_OUT2
	BCF KEY_OUT3
	BTFSC KEY_IN1
	BRA $+6
	ADDLW 8
	BRA CHK_KEY_END
	BSF KEY_OUT3
	BCF KEY_OUT4
	BTFSC KEY_IN1
	BRA $+6
	ADDLW 12
	BRA CHK_KEY_END
	ADDLW 16
	BRA CHK_KEY_END

WELKE_LIJN2
	
	BSF KEY_OUT2
	BSF KEY_OUT3
	BSF KEY_OUT4
	BSF KEY_OUT5
	BTFSC KEY_IN2
	BRA $+6
	ADDLW 0
	BRA CHK_KEY_END
	BSF KEY_OUT1
	BCF KEY_OUT2
	BTFSC KEY_IN2
	BRA $+6
	ADDLW 4
	BRA CHK_KEY_END
	BSF KEY_OUT2
	BCF KEY_OUT3
	BTFSC KEY_IN2
	BRA $+6
	ADDLW 8
	BRA CHK_KEY_END
	BSF KEY_OUT3
	BCF KEY_OUT4
	BTFSC KEY_IN2
	BRA $+6
	ADDLW 12
	BRA CHK_KEY_END
	ADDLW 16
	BRA CHK_KEY_END

WELKE_LIJN3
	
	BSF KEY_OUT2
	BSF KEY_OUT3
	BSF KEY_OUT4
	BSF KEY_OUT5
	BTFSC KEY_IN3
	BRA $+6
	ADDLW 0
	BRA CHK_KEY_END
	BSF KEY_OUT1
	BCF KEY_OUT2
	BTFSC KEY_IN3
	BRA $+6
	ADDLW 4
	BRA CHK_KEY_END
	BSF KEY_OUT2
	BCF KEY_OUT3
	BTFSC KEY_IN3
	BRA $+6
	ADDLW 8
	BRA CHK_KEY_END
	BSF KEY_OUT3
	BCF KEY_OUT4
	BTFSC KEY_IN3
	BRA $+6
	ADDLW 12
	BRA CHK_KEY_END
	ADDLW 16

	
;FLAG_KEY bit0 = 1 nieuwe toets ingedrukt
;         bit1   bij eerste maal toets ingedrukt --> 1
;         bit2   bij tweede maal toets nog steeds ingedrukt --> 1
;         bit3   bij eerste maal geen toets ingedrukt dan --> 1
;         bit4   bij tweede maal toets nog steeds niet ingedrukt --> 1		


CHK_KEY_END
	MOVWF PRESSED_KEY
	BCF KEY_OUT1
	BCF KEY_OUT2
	BCF KEY_OUT3
	BCF KEY_OUT4
	BCF KEY_OUT5
	
	BCF FLAG_KEY,4
	BCF FLAG_KEY,3
	BTFSC FLAG_KEY,1
	BRA CHK_SPR10
	BSF FLAG_KEY,1
	RETURN
CHK_SPR10
	BTFSS FLAG_KEY,2
	BSF FLAG_KEY,0
	BSF FLAG_KEY,2	
	RETURN


GEEN_KEY_INGEDRUKT

	BCF FLAG_KEY,1
	BCF FLAG_KEY,2
	BTFSC FLAG_KEY,3
	BRA CHK_SPR20
	BSF FLAG_KEY,3
	RETURN
CHK_SPR20
	BSF FLAG_KEY,4

	RETURN
	