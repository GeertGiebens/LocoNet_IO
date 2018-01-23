
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
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 28 jul 2017                      Versie LOCONET IO V1.1          *
;*                                                                         *
;* Filenaam: LOCONET_IO_INGANG.INC                                     *
;*                                                                         *
;***************************************************************************


;elke ingang werkt via massadetectie met softwarematige ontstoring en schmitt-trigger  

	#DEFINE C_LAAG 64	;onderste grens waarbij massa detectie wegvalt : teller 255 --> 0  (schmitt-trigger)
	#DEFINE C_HOOG 192	;bovenste grens waarbij massa detectie inkomt teller 0 --> 255  (schmitt-trigger)


;Waarden van BINAIR b'76543210'
;                         | |
;                         | b=0  GEEN MASSADETECTIE =1 WEL
;                         b=0 Eerste actie   =1 Tweede actie;   Gebruikt bij functie: BUTTON



;Opbouw ADRESL en ADRESH:
;
;ADRESL = '0,A6,A5,A4,A3,A2,A1,A0' 
;ADRESH = '0,0,R,0,A10,A9,A8,A7'   R =ON/OFF (1/0) bij Functie 'BUTTON ON/OFF'  Bij andere functie R=0 



MACRO_INGANG	MACRO POORTIN_NR, POORTIN,POORTINBIT

	        LOCAL MI_END, MI_SPR1,MI_SPR2,MI_SPR10, MI_SPR15, MI_SPR20, MI_SPR25, MI_SPR30, MI_SPR40, MI_SPR50, MI_SPR55, MI_SPR70, MI_SPR60, MI_SPR80
	

        BTFSS ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),0,BANKED	;Is functie ingang? NEE --> dan einde MACRO
        BRA MI_END


;********************************************************************
;********** INGANG = +5V ********************************************
;********************************************************************

        BTFSS POORTIN,POORTINBIT
        GOTO MI_SPR1

        MOVF ((LOW TBL_VAR1)+(POORTIN_NR-1)),W,BANKED
        BTFSC STATUS,Z
        GOTO MI_END                                             ;Indien VAR1 niet 0 dan VAR1 verlagen	
        DECF ((LOW TBL_VAR1)+(POORTIN_NR-1)),F,BANKED		 
        MOVF ((LOW TBL_VAR1)+(POORTIN_NR-1)),W,BANKED
        XORLW C_LAAG
        BTFSS STATUS,Z						                 
        GOTO MI_END
        BCF ((LOW TBL_BINAIR)+(POORTIN_NR-1)),1,BANKED         ;Indien VAR1=CLAAG dan RESET BIT1 (BINAIR)

        LFSR 0,TBL_ADR1L                                       ;  En laad startadres TBL_ADR1L in File Select Register 0
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F

        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR1H	    	                       ;     laad startadres TBL_ADR1H in File Select Register 0
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3


;********** FUNCTIE INGANG SWITCH  *********************************

        MOVLW b'00000001'                                      ;        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED     
        BRA MI_SPR10

        MOVLW b'00010000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER

; hier komt S88 deel
        MOVF ((LOW TBL_CONST1) + (POORTIN_NR-1)),F,BANKED    ;als C1 = 0 dan geen S88 adres
        BNZ $+6
        GOTO MI_END

        MOVLW 0xB2              
        MOVWF TE_VERZENDEN_BYTE1
        MOVLW .4
        MOVWF AANTAL_BYTES

        MOVF ((LOW TBL_CONST1) + (POORTIN_NR-1)),W,BANKED
        MOVWF TE_VERZENDEN_BYTE3
        ANDLW b'00001111'
        SWAPF WREG,W
        IORWF ((LOW TBL_CONST2) + (POORTIN_NR-1)),W,BANKED
        MOVWF TE_VERZENDEN_BYTE2
        RRNCF TE_VERZENDEN_BYTE2,F
        BCF TE_VERZENDEN_BYTE2,7
        SWAPF TE_VERZENDEN_BYTE2,W
        ANDLW b'00001111'
        MOVWF TE_VERZENDEN_BYTE3
        BTFSC ((LOW TBL_CONST2) + (POORTIN_NR-1)),0,BANKED
        BSF TE_VERZENDEN_BYTE3,5
        CALL TE_VERZENDEN_BYTES_IN_BUFFER

        GOTO MI_END


;********** FUNCTIE INGANG BUTTON  **********************************

MI_SPR10
        MOVLW b'00000011'        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED      
        BRA MI_SPR20
        BTFSC ((LOW TBL_BINAIR)+(POORTIN_NR-1)),3,BANKED       
        BRA MI_SPR15
        BSF ((LOW TBL_BINAIR)+(POORTIN_NR-1)),3,BANKED
        MOVLW b'00100000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END
MI_SPR15
        BCF ((LOW TBL_BINAIR)+(POORTIN_NR-1)),3,BANKED
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END


;********** FUNCTIE INGANG BUTTON ON  *********************************

MI_SPR20
        MOVLW b'00000101'        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED
        BRA MI_SPR30
        MOVLW b'00100000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END


;********** FUNCTIE INGANG BUTTON OFF  *********************************

MI_SPR30
        MOVLW b'00000111'        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED
        BRA MI_SPR40
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END


;********** FUNCTIE INGANG ON/OFF  **********************************

MI_SPR40
        MOVLW b'00100001'                                     
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED
        GOTO MI_END


;********** FUNCTIE ON/OFF ADR1 **************

        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),0,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR1L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR1H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),1,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER

;********** FUNCTIE ON/OFF ADR2 **************



        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),2,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR2L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR2H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),3,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR3 **************

        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),4,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR3L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR3H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),5,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR4 **************

        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),6,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR4L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR4H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),7,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR5 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),0,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR5L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR5H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),1,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR6 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),2,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR6L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR6H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),3,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR7 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),4,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR7L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR7H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),5,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR8 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),6,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR8L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR8H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),7,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR9 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),0,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR9L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR9H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),1,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR10 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),2,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR10L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR10H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),3,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
    	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR11 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),4,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR11L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR11H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),5,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
    	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR12 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),6,BANKED   
        BRA $+30
        LFSR 0,TBL_ADR12L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR12H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),7,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
    	CALL TE_VERZENDEN_BYTES_IN_BUFFER

;**********************************************************
;********** INGANG = MASSA ********************************
;**********************************************************

MI_SPR1
        INCFSZ ((LOW TBL_VAR1)+(POORTIN_NR-1)),F,BANKED		; indien ingang laag (massa)dan teller verhogen
        BRA MI_SPR2
        DECF ((LOW TBL_VAR1)+(POORTIN_NR-1)),F,BANKED		; 0xFF blijft de hoogste mogelijke waarde
        GOTO MI_END
MI_SPR2	
        MOVF ((LOW TBL_VAR1)+(POORTIN_NR-1)),W,BANKED
        XORLW C_HOOG
        BTFSS STATUS,Z						; indien teller=CHOOG dan SET BIT1 (BINAIR) EN STUUR DATA naar TABEL te VERZENDEN BYTE
        GOTO MI_END
        BSF ((LOW TBL_BINAIR)+(POORTIN_NR-1)),1,BANKED

    	LFSR 0,TBL_ADR1L	                            	;Laad startadres TBL_ADR1L in File Select Register 0
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
		LFSR 0,TBL_ADR1H                            	;Laad startadres TBL_ADR1H in File Select Register 0
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3


;********** FUNCTIE INGANG SWITCH  *********************************

        MOVLW b'00000001'        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED
        BRA MI_SPR50

        MOVLW b'00110000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER

;hier komt S88 deel
        MOVF ((LOW TBL_CONST1) + (POORTIN_NR-1)),F,BANKED    ;als C1 = 0 dan geen S88 adres
        BNZ $+6
        GOTO MI_END

        MOVLW 0xB2              
        MOVWF TE_VERZENDEN_BYTE1
        MOVLW .4
        MOVWF AANTAL_BYTES

        MOVF ((LOW TBL_CONST1) + (POORTIN_NR-1)),W,BANKED
        MOVWF TE_VERZENDEN_BYTE3
        ANDLW b'00001111'
        SWAPF WREG,W
        IORWF ((LOW TBL_CONST2) + (POORTIN_NR-1)),W,BANKED
        MOVWF TE_VERZENDEN_BYTE2
        RRNCF TE_VERZENDEN_BYTE2,F
        BCF TE_VERZENDEN_BYTE2,7
        SWAPF TE_VERZENDEN_BYTE2,W
        ANDLW b'00001111'
        MOVWF TE_VERZENDEN_BYTE3
        BTFSC ((LOW TBL_CONST2) + (POORTIN_NR-1)),0,BANKED
        BSF TE_VERZENDEN_BYTE3,5
        BSF TE_VERZENDEN_BYTE3,4
        CALL TE_VERZENDEN_BYTES_IN_BUFFER

        GOTO MI_END


;********** FUNCTIE INGANG BUTTON  *********************************

MI_SPR50
        MOVLW b'00000011'        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED 
        BRA MI_SPR60
        BTFSC ((LOW TBL_BINAIR)+(POORTIN_NR-1)),3,BANKED  
        BRA MI_SPR55
        MOVLW b'00110000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END
MI_SPR55
        MOVLW b'00010000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END


;********** FUNCTIE INGANG BUTTON ON  *********************************

MI_SPR60
        MOVLW b'00000101'        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED
        BRA MI_SPR70
        MOVLW b'00110000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER

;hier komt OPC_GPON
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),0,BANKED
        GOTO MI_END
        MOVLW 0x83
        MOVWF TE_VERZENDEN_BYTE1
        MOVLW 2
        MOVWF AANTAL_BYTES
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END


;********** FUNCTIE INGANG BUTTON OFF  *********************************

MI_SPR70
        MOVLW b'00000111'        
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED
        BRA MI_SPR80
        MOVLW b'00010000'
        IORWF TE_VERZENDEN_BYTE3,F
        CALL TE_VERZENDEN_BYTES_IN_BUFFER

;hier komt OPC_GPOFF
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),0,BANKED
        GOTO MI_END
        MOVLW 0x82
        MOVWF TE_VERZENDEN_BYTE1
        MOVLW 2
        MOVWF AANTAL_BYTES
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        GOTO MI_END




;********** FUNCTIE INGANG ON/OFF  **********************************

MI_SPR80
        MOVLW b'00100001'                                     
        CPFSEQ((LOW TBL_FUNCTIE) + (POORTIN_NR-1)),BANKED
        GOTO MI_END


;********** FUNCTIE ON/OFF ADR1 **************

        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),0,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR1L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR1H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),1,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER

;********** FUNCTIE ON/OFF ADR2 **************



        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),2,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR2L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR2H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),3,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR3 **************

        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),4,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR3L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR3H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),5,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR4 **************

        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),6,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR4L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR4H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST1) + (POORTIN_NR-1)),7,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR5 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),0,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR5L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR5H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),1,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR6 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),2,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR6L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR6H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),3,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR7 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),4,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR7L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR7H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),5,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR8 **************

        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),6,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR8L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR8H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST2) + (POORTIN_NR-1)),7,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR9 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),0,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR9L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR9H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),1,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR10 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),2,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR10L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR10H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),3,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
    	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR11 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),4,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR11L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR11H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),5,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
    	CALL TE_VERZENDEN_BYTES_IN_BUFFER


;********** FUNCTIE ON/OFF ADR12 **************

        BTFSS ((LOW TBL_CONST3) + (POORTIN_NR-1)),6,BANKED   
        BRA $+32
        LFSR 0,TBL_ADR12L
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE2
        LFSR 0,TBL_ADR12H                            	
        MOVLW (POORTIN_NR-1)
        ADDWF FSR0L,F
        MOVF INDF0,W
        MOVWF TE_VERZENDEN_BYTE3
        BSF TE_VERZENDEN_BYTE3,4
        BTFSS ((LOW TBL_CONST3)+ (POORTIN_NR-1)),7,BANKED   
        BRA $+4
        BSF TE_VERZENDEN_BYTE3,5
    	CALL TE_VERZENDEN_BYTES_IN_BUFFER

	GOTO MI_END


MI_END	
        NOP
        ENDM


;********************************************************************

TEST_INGANG

        MOVLW 0xB0              ;indien nodig staan deze waarden al juist
        MOVWF TE_VERZENDEN_BYTE1
        MOVLW .4
        MOVWF AANTAL_BYTES


	MOVLB HIGH TBL_FUNCTIE



	MACRO_INGANG 1,POORT1
	MACRO_INGANG 2,POORT2
	MACRO_INGANG 3,POORT3
	MACRO_INGANG 4,POORT4
	MACRO_INGANG 5,POORT5
	MACRO_INGANG 6,POORT6
	MACRO_INGANG 7,POORT7
	MACRO_INGANG 8,POORT8
	MACRO_INGANG 9,POORT9
	MACRO_INGANG 10,POORT10
	MACRO_INGANG 11,POORT11
	MACRO_INGANG 12,POORT12
	MACRO_INGANG 13,POORT13
	MACRO_INGANG 14,POORT14
	MACRO_INGANG 15,POORT15
	MACRO_INGANG 16,POORT16
	MACRO_INGANG 17,POORT17
	MACRO_INGANG 18,POORT18
	MACRO_INGANG 19,POORT19
	MACRO_INGANG 20,POORT20
	MACRO_INGANG 21,POORT21
	MACRO_INGANG 22,POORT22
	MACRO_INGANG 23,POORT23
	MACRO_INGANG 24,POORT24
	MACRO_INGANG 25,POORT25
	MACRO_INGANG 26,POORT26
	MACRO_INGANG 27,POORT27
	MACRO_INGANG 28,POORT28

    IF DEBUG == 0 
	MACRO_INGANG 29,POORT29
	MACRO_INGANG 30,POORT30
    ENDIF


	RETURN

