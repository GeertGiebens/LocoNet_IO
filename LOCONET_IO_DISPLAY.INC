
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
;* Datum: 21 mei 2017                      Versie LOCONET IO V1.1          *
;*                                                                         *
;* Filenaam: LOCONET_IO_DISPLAY.INC                                        *
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


;LCD:

;GND    = pin1
;+5V    = pin2
;GND    = pin3
;LCD_RS = pin4
;GND    = pin5
;LCD_E  = pin6
;GND    = pin7
;GND    = pin8
;GND    = pin9
;GND    = pin10
;LCD_D4 = pin11
;LCD_D5 = pin12
;LCD_D6 = pin13
;LCD_D7 = pin14

    #DEFINE CURSOR_FREQ              24           ;Knipper frequentie CURSOR =   FREQUENTIE = 30/CURSOR_FREQ (Hz)



    #DEFINE UPDATE_DISPLAY      DISPLAY_FLAG,1      ; = '1' Update display gewenst met nieuwe DATA
    #DEFINE DISPLAY_ACTIEF      DISPLAY_FLAG,0      ; = '1' Dan DISPLAY en TOETSENBORD (KEYBOARD) actief
    #DEFINE NIEUW_DISPLAY_DATA  DISPLAY_FLAG,2      ; = '1' 
    #DEFINE CURSOR_BIT          DISPLAY_FLAG,3      ; = '1' CURSOR ON
    #DEFINE CURSOR_KNIPPER      DISPLAY_FLAG,4      ; = '1' CURSOR MOET KNIPPEREN

    #DEFINE LCD_D7                  LATA,1  ;(uitgang DISPLAY)
    #DEFINE LCD_D6                  LATA,2  ;(uitgang DISPLAY)
    #DEFINE LCD_D5                  LATA,3  ;(uitgang DISPLAY)
    #DEFINE LCD_D4                  LATA,4  ;(uitgang DISPLAY)
    #define LCD_E                   LATA,5
    #define LCD_RS                  LATE,0





;******************************************************
;*       binary_to_bcd, From: Scott Dattalo
;******************************************************

;aar0 = binary number low byte
;aar1 = binary number high byte
;aac0 = bcd number ones
;aac1 = bcd number tens
;aac2 = bcd number hundreds
;aac3 = bcd number thousands
;aac4 = bcd number ten-thousands
 
b16_d5
        swapf   aar0,w	; partial ones sum in low byte
        addwf   aar0,w	;
        andlw   0x0f	;
        btfsc STATUS,DC		;
        addlw 	0x16	;
        btfsc STATUS,DC			;
        addlw  	0x06	;
        addlw   0x06	;
        btfss STATUS,DC			;
    	addlw  	-0x06	; wmax=3:0
;
        btfsc   aar0,4	; complete ones sum in low byte
        addlw   0x15+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=4:5
    	movwf	aac0	; save sum in aac0
;
;     8+      4+     2+     1+     8+     4+    2+    1+
;    20
;   100      60     30     15+
;   ----------------------------------------------------
;   128      64     32     16      8      4     2     1
;
        swapf   aar1,w	; partial ones sum in high byte
        addwf   aar1,w	;
        andlw   0x0f	;
        btfsc STATUS,DC		;
        addlw 	0x16	;
        btfsc STATUS,DC		;
        addlw  	0x06	;
        addlw   0x06	;
        btfss STATUS,DC			;
	addlw  	-0x06	; wmax=3:0
;
	btfsc   aar1,0	; complete ones sum in high byte
        addlw   0x05+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=3:5
;
        btfsc   aar1,4
        addlw   0x15+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=5:0
;
	addlw	0x06	; include previous sum
	addwf	aac0,w
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=9:5, ones sum ended
;
	movwf	aac0
	movwf	aac1
	swapf	aac1,f
	movlw	0x0f
	andwf	aac0,f	; save total ones sum in aac0
	andwf	aac1,f	; save partial tens sum in aac1
;
;     8+      4+     2+     1+     8+     4+    2+    1+
;                           5+
;    60      80     90     10+                        5+
;   700     300    100     80     40     20    10    50 
; 32000   16000   8000   4000   2000   1000   500   200
; ------------------------------------------------------
; 32768   16384   8192   4096   2048   1024   512   256
;
			; complete tens sum in low and high byte
	rrcf	aar1,w	; rotate right high byte once
	andlw	0x0f	; clear high nibble
	addlw	0x06	; adjust bcd
        btfss STATUS,DC	
	addlw	-0x06	; wmax=1:5
;
	addlw	0x06	; include previous sum
	addwf	aac1,w
        btfss STATUS,DC	 
        addlw  	-0x06	; wmax=2:4
;
        btfsc   aar0,5
        addlw   0x03+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=2:7
;
        btfsc   aar0,6
        addlw   0x06+0x06
        btfss STATUS,DC	 
        addlw  	-0x06	; wmax=3:3
;
        btfsc   aar0,7
        addlw   0x12+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=4:5
;
        btfsc   aar1,0
        addlw   0x25+0x06
        btfss STATUS,DC	 
        addlw  	-0x06	; wmax=7:0
;
        btfsc   aar1,5
        addlw   0x09+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=7:9
;
        btfsc   aar1,6
        addlw   0x08+0x06
        btfss STATUS,DC	 
        addlw  	-0x06	; wmax=8:7
;
        btfsc   aar1,7
        addlw   0x06+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=9:3, tens sum ended
;
	movwf	aac1	; save total tens sum in aac1
	swapf	aac1,w
	andlw	0x0f	; load partial hundreds sum in w
;
;     8+      4+     2+     1+     8+     4+    2+    1+
;    20+                    5+
;   100+     60+    30+    10+
;   ----------------------------------------------------
;   128      64     32     16      8      4     2     1
;
;     8+      4+     2+     1+     8+     4+    2+    1+
;                           5+
;    60+     80+    90+    10+                        5+
;   700     300    100     80+    40+    20+   10+   50+
; 32000   16000   8000   4000   2000   1000   500   200+
; ------------------------------------------------------
; 32768   16384   8192   4096   2048   1024   512   256
;
			; complete hundreds sum in high byte
        btfsc   aar1,1
        addlw   0x05+0x06
        btfss STATUS,DC	 
        addlw  	-0x06	; wmax=1:4
;
        btfsc   aar1,5
        addlw   0x01+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=1:5
;
        btfsc   aar1,6
        addlw   0x03+0x06
        btfss STATUS,DC	 
        addlw  	-0x06	; wmax=1:8
;
        btfsc   aar1,7
        addlw   0x07+0x06
        btfss STATUS,DC	
        addlw  	-0x06	; wmax=2:5, hundreds sum ended
;
	movwf	aac2	; save total hundreds sum in aac2
	swapf	aac2,w
	movwf	aac3	; save partial thousands sum in aac3	
	movlw	0x0f	; clear high nibble
	andwf	aac1,f	;
	andwf	aac2,f	;
	andwf	aac3,f	;
;
;     8+      4+     2+     1+     8+     4+    2+    1+
;                           5+
;    60+     80+    90+    10+                        5+
;   700+    300+   100+    80+    40+    20+   10+   50+
; 32000   16000   8000   4000   2000   1000   500+  200+
; ------------------------------------------------------
; 32768   16384   8192   4096   2048   1024   512   256
;
			; complete thousands sum in low and high byte
	rrcf	aar1,w	; rotate right high byte twice
	movwf	aac4	;
	rrcf	aac4,w	;
	andlw	0x0f	; clear high nibble
	addlw	0x06	; adjust bcd
        btfss STATUS,DC			;
	addlw	-0x06	; wmax=1:5
;
	addlw	0x06	; include previous sum
	addwf	aac3,w
        btfss STATUS,DC	
	addlw	-0x06	; wmax=1:7
;
        btfsc   aar1,6
        addlw   0x16+0x06
        btfss STATUS,DC	
	addlw	-0x06	; wmax=3:3
;
        btfsc   aar1,7
        addlw   0x32+0x06
        btfss STATUS,DC	
	addlw	-0x06	; wmax=6:5, thousands sum ended
;
	movwf	aac3	; save total thousands sum in aac3
	movwf	aac4	;
	swapf	aac4,f	; save ten-thousands sum in aac4
	movlw	0x0f	; clear high nibble
	andwf	aac3,f	;
	andwf	aac4,f	;
	
	BSF aac0,4		;maak er een ASCII code van (Geert Giebens)
	BSF aac0,5
	BSF aac1,4
	BSF aac1,5
	BSF aac2,4
	BSF aac2,5
	BSF aac3,4
	BSF aac3,5
	BSF aac4,4
	BSF aac4,5

    return

	
	
;********************************************************************
;*       1ms vertraging x waarde in W-register FROM: Stefan Johansson
;********************************************************************

DELAY_W_MS

        MOVWF MS
        CLRF D1
DELAY_SPR1
        MOVLW .50; 11
        MOVWF D2
DELAY_SPR2
        DECFSZ D1,F
        BRA $-2
        DECFSZ D2,F
        BRA DELAY_SPR2
        DECFSZ MS,F
        BRA DELAY_SPR1
        RETURN


;********************************************************************
;*       40µs vertraging  FROM: Stefan Johansson
;********************************************************************

DELAY_40

    MOVLW  .106
    MOVWF D1
    DECFSZ D1,F
    BRA $-2
    RETURN

;********************************************************************
;*       PULSE  FROM: Stefan Johansson
;********************************************************************

PULSE
    BSF LCD_E
    BRA $+2
    BRA $+2
    BRA $+2
    BRA $+2
    BCF LCD_E
    CALL DELAY_40

    RETURN


;********************************************************************
;*      Waarde W-register naar LCD   FROM: Stefan Johansson
;********************************************************************

LCD_SEND   ;(Data staat in W-register)

    MOVWF DISPLAY_DATA

    BCF LCD_D4
    BCF LCD_D5
    BCF LCD_D6
    BCF LCD_D7
    BTFSC DISPLAY_DATA,4
    BSF LCD_D4
    BTFSC DISPLAY_DATA,5
    BSF LCD_D5
    BTFSC DISPLAY_DATA,6
    BSF LCD_D6
    BTFSC DISPLAY_DATA,7
    BSF LCD_D7
    CALL PULSE

    BCF LCD_D4
    BCF LCD_D5
    BCF LCD_D6
    BCF LCD_D7
    BTFSC DISPLAY_DATA,0
    BSF LCD_D4
    BTFSC DISPLAY_DATA,1
    BSF LCD_D5
    BTFSC DISPLAY_DATA,2
    BSF LCD_D6
    BTFSC DISPLAY_DATA,3
    BSF LCD_D7
    CALL PULSE

    RETURN

;********************************************************************
;*      LCD CLEAR   FROM: Stefan Johansson
;********************************************************************

LCD_CLEAR
    BCF LCD_RS
    MOVLW b'00000001'
    CALL LCD_SEND

    MOVLW .5
    CALL DELAY_W_MS

    MOVLW b'00000010'
    CALL LCD_SEND

    MOVLW .2
    CALL DELAY_W_MS


    BSF LCD_RS
    RETURN

	
;********************************************************************
;*      LCD INIT   FROM: Stefan Johansson
;********************************************************************

INIT_LCD

    CLRF DISPLAY_FLAG
    BSF TRISA,0             ;PORTA,0='0'  dan selecteer DISPLAY en KEYBOARD
    BTFSC PORTA,0
    RETURN
    BSF DISPLAY_ACTIEF


    BCF TRISA,1
    BCF TRISA,2
    BCF TRISA,3
    BCF TRISA,4
    BCF TRISA,5
    BCF TRISE,0

    BCF LCD_RS
    BCF LCD_E
    BSF LCD_D4
    BSF LCD_D5
    BCF LCD_D6
    BCF LCD_D7

    MOVLW .255
    CALL DELAY_W_MS

    CALL PULSE
 
    MOVLW .5
    CALL DELAY_W_MS

    CALL PULSE

    MOVLW .5
    CALL DELAY_W_MS

    CALL PULSE

    MOVLW .5
    CALL DELAY_W_MS

    BCF LCD_D4
    CALL PULSE

    MOVLW b'00101000'
    CALL LCD_SEND

    MOVLW b'00001110'
    CALL LCD_SEND

    MOVLW b'00000110'		;Increment Cursor
    CALL LCD_SEND
	
    MOVLW b'00010000'
    CALL LCD_SEND

    MOVLW b'00000001'
    CALL LCD_SEND

    MOVLW 4
    CALL DELAY_W_MS

    BSF LCD_RS

    CALL LCD_CLEAR

    RETURN
	
	
;********************************************************************
;*      CURSOR_BLINKING   
;********************************************************************

;Elke 16,66ms (60 keer per seconde)

KNIPPER_CURSOR

        BTFSS  DISPLAY_ACTIEF
    	RETURN

        DECFSZ CURSORTEL,F         ;verminder CURSORTEL als CURSORTEL =0 dan reset teller en toggle CURSOR
        RETURN
        MOVLW CURSOR_FREQ
        MOVWF CURSORTEL 
        BTG CURSOR_BIT
        BCF LCD_RS
        BTFSS CURSOR_BIT
        BRA KN_SPR5
        MOVLW b'00001110'           ; cursor ON
        CALL LCD_SEND
        RETURN
KN_SPR5
        BTFSS CURSOR_KNIPPER
        RETURN
        MOVLW b'00001100'           ; cursor OFF
        CALL LCD_SEND              				
        RETURN

		
	
;********************************************************************
;*     DATA (BINAIR) NAAR DISPLAY_R1 of DISPLAY_R2
;********************************************************************

;Binaire DATA omzetten naar ASCII en deze op gevraagde positie plaatsen in DISPLAY_R1 of DISPLAY_R2

;DISPLAY_POS =b'RUAAPPPP'
;               R        R=0 RIJ1;  R=1 RIJ2
;                U       U=1 Underscore op positie PPPP 
;                 AA     =Aantal posities waarin DATA moet verschijnen in DISPLAY vertrekkende van links 1(00); 2(01); 3(10) of 4(11)
;                   PPPP =Startpositie voor plaatsing DATA 0--> 15

;aar0 = binary number low byte
;aar1 = binary number high byte
;aac0 = bcd number ones
;aac1 = bcd number tens
;aac2 = bcd number hundreds
;aac3 = bcd number thousands
;aac4 = bcd number ten-thousands

DATA_NAAR_DISPLAY

    CALL b16_d5

    MOVF DISPLAY_POS,W      ;plaats cursor
    ANDLW b'00001111'
    BSF WREG,7
    BTFSC DISPLAY_POS,7
    BSF WREG,6
    BCF LCD_RS
    CALL LCD_SEND
    MOVLW b'00000100'       ;cursor decrement
    CALL LCD_SEND
    BSF LCD_RS

    MOVF DISPLAY_POS,W
    SWAPF WREG
    ANDLW b'00000011'
    MOVWF HULPTEL
    INCF HULPTEL,F
    MOVF aac0,W
    IORLW b'00110000'
    CALL LCD_SEND
    DECFSZ HULPTEL,F
    BRA $+4
    BRA DND_END
    MOVF aac1,W
    IORLW b'00110000'
    CALL LCD_SEND
    DECFSZ HULPTEL,F
    BRA $+4
    BRA DND_END
    MOVF aac2,W
    IORLW b'00110000'
    CALL LCD_SEND
    DECFSZ HULPTEL,F
    BRA $+4
    BRA DND_END
    MOVF aac3,W
    IORLW b'00110000'
    CALL LCD_SEND

DND_END
    MOVF DISPLAY_POS,W      ;plaats cursor
    ANDLW b'00001111'
    BSF WREG,7
    BTFSC DISPLAY_POS,7
    BSF WREG,6
    BCF LCD_RS
    CALL LCD_SEND
    MOVLW b'00001110'
    CALL LCD_SEND
    BSF LCD_RS
    BCF CURSOR_KNIPPER      
    BTFSC DISPLAY_POS,6
    BSF CURSOR_KNIPPER 

    RETURN

		;*****MAG WEG
	
	
;********************************************************************
;*     DISPLAY DEVICE NUMMER (VAR: DEVICENR)
;********************************************************************
	
	; ----------------
	;|#001            |
	;|                |
	; ----------------
		
DISPLAY_DEVICE_NR

	MOVF DEVICENR,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0x80			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'#'
	CALL LCD_SEND
	MOVF aac2,W
	CALL LCD_SEND
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0x83			;plaats cursor
	CALL LCD_SEND
	
	RETURN
	

;********************************************************************
;*     DISPLAY POORT NUMMER (VAR: POORTNR)
;********************************************************************
	
	; ----------------
	;|     *01        |
	;|                |
	; ----------------
		
DISPLAY_POORT_NR

	MOVF POORTNR,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0x85			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'*'
	CALL LCD_SEND
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0x87			;plaats cursor
	CALL LCD_SEND
	
	RETURN
	
	
;********************************************************************
;*     DISPLAY FUNCTIE (VAR: FUNCTIE)
;********************************************************************
	
	; ----------------
	;|         FUNCTIE|
	;|                |
	; ----------------
		
DISPLAY_FUNCTIE
	
	BCF LCD_RS
	MOVLW 0x89			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW FUNCTIE_NOP
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_NOP
	
	MOVLW FUNCTIE_SWITCH
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_SWITCH
	
	MOVLW FUNCTIE_BUTTON
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_BUTTON
	
	MOVLW FUNCTIE_BUTTON_ON
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_BUTTON_ON
	
	MOVLW FUNCTIE_BUTTON_OFF
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_BUTTON_OFF
	
	MOVLW FUNCTIE_RELAY
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_RELAY
	
	MOVLW FUNCTIE_COIL1
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_COIL1
	
	MOVLW FUNCTIE_COIL2
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_COIL2
	
	MOVLW FUNCTIE_LED
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_LED
	
	MOVLW FUNCTIE_LED_BLK
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_LED_BLK
	
	MOVLW FUNCTIE_SERVO
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_SERVO
	
	MOVLW FUNCTIE_ON_OFF
	CPFSEQ FUNCTIE
	BRA $+4
	BRA DISPLAY_FUNCTIE_ON_OFF
	
	RETURN
	
	
DISPLAY_FUNCTIE_NOP	
	
	MOVLW A'N'
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'P'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_SWITCH
	
	MOVLW A'S'
	CALL LCD_SEND
	MOVLW A'W'
	CALL LCD_SEND
	MOVLW A'I'
	CALL LCD_SEND
	MOVLW A'T'
	CALL LCD_SEND
	MOVLW A'C'
	CALL LCD_SEND
	MOVLW A'H'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_BUTTON
	
	MOVLW A'B'
	CALL LCD_SEND
	MOVLW A'U'
	CALL LCD_SEND
	MOVLW A'T'
	CALL LCD_SEND
	MOVLW A'T'
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'N'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_BUTTON_ON
	
	MOVLW A'B'
	CALL LCD_SEND
	MOVLW A'U'
	CALL LCD_SEND
	MOVLW A'T'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'N'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_BUTTON_OFF
	
	MOVLW A'B'
	CALL LCD_SEND
	MOVLW A'U'
	CALL LCD_SEND
	MOVLW A'T'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'F'
	CALL LCD_SEND
	MOVLW A'F'
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_RELAY
	
	MOVLW A'R'
	CALL LCD_SEND
	MOVLW A'E'
	CALL LCD_SEND
	MOVLW A'L'
	CALL LCD_SEND
	MOVLW A'A'
	CALL LCD_SEND
	MOVLW A'Y'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_COIL1
	
	MOVLW A'C'
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'I'
	CALL LCD_SEND
	MOVLW A'L'
	CALL LCD_SEND
	MOVLW A'1'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_COIL2
	
	MOVLW A'C'
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'I'
	CALL LCD_SEND
	MOVLW A'L'
	CALL LCD_SEND
	MOVLW A'2'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_LED
	
	MOVLW A'L'
	CALL LCD_SEND
	MOVLW A'E'
	CALL LCD_SEND
	MOVLW A'D'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_LED_BLK
	
	MOVLW A'L'
	CALL LCD_SEND
	MOVLW A'E'
	CALL LCD_SEND
	MOVLW A'D'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A'B'
	CALL LCD_SEND
	MOVLW A'L'
	CALL LCD_SEND
	MOVLW A'K'
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_SERVO
	
	MOVLW A'S'
	CALL LCD_SEND
	MOVLW A'E'
	CALL LCD_SEND
	MOVLW A'R'
	CALL LCD_SEND
	MOVLW A'V'
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	
DISPLAY_FUNCTIE_ON_OFF
	
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'N'
	CALL LCD_SEND
	MOVLW A'/'
	CALL LCD_SEND
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'F'
	CALL LCD_SEND
	MOVLW A'F'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND
	BRA DF_END
	

DF_END	
	BCF LCD_RS
	MOVLW 0x89			;plaats cursor
	CALL LCD_SEND
	
	RETURN	
	
	
;********************************************************************
;*     DISPLAY ADRES (VAR: ADRESNR; ADRESLOW; ADRESHIGH)
;********************************************************************
	
	; ----------------
	;|                |
	;| A01:0001       |
	; ----------------
	
;ADRESL = 'X,A6,A5,A4,A3,A2,A1,A0' X=0 Adres aanwezig in buffer     X=1 Geen adres aanwezig in buffer
;ADRESH = '0,0,R,0,A10,A9,A8,A7'   R =ON/OFF (1/0) bij Functie 'ON/OFF'  Bij andere functie R=0   
		
DISPLAY_ADRES

	MOVF ADRESNR,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xC0			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'A'
	CALL LCD_SEND
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	MOVLW A':'
	CALL LCD_SEND
	
	MOVF ADRESLOW,W
	BCF WREG,7
	BTFSC ADRESHIGH,0
	BSF WREG,7
	MOVWF aar1
	MOVF ADRESHIGH,W
	MOVWF aar1
	BCF aar1,5
	BCF STATUS,C
	RRCF aar1,F
	CALL b16_d5
	
	MOVF aac3,W
	CALL LCD_SEND
	MOVF aac2,W
	CALL LCD_SEND
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xC7			;plaats cursor
	CALL LCD_SEND
	
	RETURN
	
;********************************************************************
;*     DISPLAY ON/OFF (VAR: ADRESHIGH)
;********************************************************************
	
	; ----------------
	;|                |
	;|          OFF   |
	; ----------------
		
DISPLAY_ON_OFF


	
	BCF LCD_RS
	MOVLW 0xC9			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	BTFSC ADRESHIGH,5
	BRA DDN_SPR1
	
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'F'
	CALL LCD_SEND
	MOVLW A'F'
	CALL LCD_SEND
	BRA DDN_SPR2

DDN_SPR1
	MOVLW A'O'
	CALL LCD_SEND
	MOVLW A'N'
	CALL LCD_SEND
	MOVLW A' '
	CALL LCD_SEND	
	
DDN_SPR2	
	BCF LCD_RS
	MOVLW 0xC9			;plaats cursor
	CALL LCD_SEND
	
	RETURN

	
;********************************************************************
;*     DISPLAY CONST1 m (VAR: CONST1)
;********************************************************************
	
	; ----------------
	;|                |
	;|m050            |
	; ----------------
		
DISPLAY_CONST1_m

	MOVF CONST1,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xC0			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'm'
	CALL LCD_SEND
	MOVF aac2,W
	CALL LCD_SEND
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xC3			;plaats cursor
	CALL LCD_SEND
	
	RETURN
	
	
;********************************************************************
;*     DISPLAY CONST2 M (VAR: CONST2)
;********************************************************************
	
	; ----------------
	;|                |
	;|     M250       |
	; ----------------
		
DISPLAY_CONST2_M

	MOVF CONST2,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xC5			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'M'
	CALL LCD_SEND
	MOVF aac2,W
	CALL LCD_SEND	
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xC8			;plaats cursor
	CALL LCD_SEND
	
	RETURN	
	
	
;********************************************************************
;*     DISPLAY CONST3 T (VAR: CONST3)
;********************************************************************
	
	; ----------------
	;|                |
	;|          T001  |
	; ----------------
		
DISPLAY_CONST3_T

	MOVF CONST3,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xCA			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'T'
	CALL LCD_SEND
	MOVF aac2,W
	CALL LCD_SEND	
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xCD			;plaats cursor
	CALL LCD_SEND
	
	RETURN		

	
;********************************************************************
;*     DISPLAY CONST1 T1 (VAR: CONST1)
;********************************************************************
	
	; ----------------
	;|                |
	;|T1:000          |
	; ----------------
		
DISPLAY_CONST1_T1

	MOVF CONST1,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xC0			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'T'
	CALL LCD_SEND
	MOVLW A'1'
	CALL LCD_SEND
	MOVLW A':'
	CALL LCD_SEND	
	MOVF aac2,W
	CALL LCD_SEND
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xC5			;plaats cursor
	CALL LCD_SEND
	
	RETURN	
	

;********************************************************************
;*     DISPLAY CONST1 T2 (VAR: CONST2)
;********************************************************************
	
	; ----------------
	;|                |
	;|        T2:000  |
	; ----------------
		
DISPLAY_CONST2_T2

	MOVF CONST2,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xC7			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'T'
	CALL LCD_SEND
	MOVLW A'2'
	CALL LCD_SEND
	MOVLW A':'
	CALL LCD_SEND	
	MOVF aac2,W
	CALL LCD_SEND
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xCC			;plaats cursor
	CALL LCD_SEND
	
	RETURN	


;********************************************************************
;*     DISPLAY SV (VAR: CONST1)
;********************************************************************
	
	; ----------------
	;|         SV:001 |
	;|                |
	; ----------------
		
DISPLAY_SV

	MOVF CONST1,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0x89			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'S'
	CALL LCD_SEND
	MOVLW A'V'
	CALL LCD_SEND
	MOVLW A':'
	CALL LCD_SEND	
	MOVF aac2,W
	CALL LCD_SEND	
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0x8E			;plaats cursor
	CALL LCD_SEND
	
	RETURN
	
	
;********************************************************************
;*     DISPLAY S88 POORT (VAR: ADRESLOW)
;********************************************************************
	
	; ----------------
	;|                |
	;|       -16      |
	; ----------------
		
DISPLAY_S88_POORT

	MOVF ADRESLOW,W
	ANDLW b'00001111'
	INCF WREG,W
	MOVWF aar0
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xC7			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'-'
	CALL LCD_SEND	
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xC9			;plaats cursor
	CALL LCD_SEND
	
	RETURN	


;********************************************************************
;*     DISPLAY S88 DEVICE (VAR: ADRESLOW; ADRESHIGH)
;********************************************************************
	
	; ----------------
	;|                |
	;|S88:001         |
	; ----------------
		
DISPLAY_S88_DEVICE

	MOVF ADRESLOW,W
	ANDLW b'11110000'
	MOVWF aar0
	SWAPF aar0,F
	BTFSC ADRESHIGH,0
	BSF aar0,3
	BTFSC ADRESHIGH,1
	BSF aar0,4
	BTFSC ADRESHIGH,2
	BSF aar0,5
	BTFSC ADRESHIGH,3
	BSF aar0,6	
	INCF aar0,F	
	CLRF aar1
	CALL b16_d5
	
	BCF LCD_RS
	MOVLW 0xC0			;plaats cursor
	CALL LCD_SEND
	BSF LCD_RS
	
	MOVLW A'S'
	CALL LCD_SEND
	MOVLW A'8'
	CALL LCD_SEND
	MOVLW A'8'
	CALL LCD_SEND
	MOVLW A':'
	CALL LCD_SEND	
	MOVF aac2,W
	CALL LCD_SEND	
	MOVF aac1,W
	CALL LCD_SEND
	MOVF aac0,W
	CALL LCD_SEND
	
	BCF LCD_RS
	MOVLW 0xC6			;plaats cursor
	CALL LCD_SEND
	
	RETURN		