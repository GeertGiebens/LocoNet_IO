
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
;* Datum: 12 aug 2017                      Versie LOCONET IO V1.1          *
;*                                                                         *
;* Filenaam: LOCONET_DECODEER_V1p1.INC                                     *
;*                                                                         *
;***************************************************************************



DECODEER_LOCONET_DATA

		BTFSS LN_NIEUWDATA		;Is er nieuwe LocoNet Data? Zo ja decodeer.
		RETURN
        BCF LN_NIEUWDATA


		MOVLB HIGH LN_RECEIVE_BUFFER
		MOVF LOW LN_RECEIVE_BUFFER,W,BANKED
		MOVWF LN_BYTE_RECEIVE1
		MOVF (LOW LN_RECEIVE_BUFFER)+1,W,BANKED
		MOVWF LN_BYTE_RECEIVE2
		MOVF (LOW LN_RECEIVE_BUFFER)+2,W,BANKED
		MOVWF LN_BYTE_RECEIVE3





;***************************************************************************

;LN_BYTE_RECEIVE1=       Opcode OPC_SW_REQ (B0)?
;LN_BYTE_RECEIVE2=	     SW1 = b'0,A6,A5,A4,A3,A2,A1,A0'
;LN_BYTE_RECEIVE3=       SW2 = b'0,0,DIR,ON,A10,A9,A8,A7'

;CODE FUNCTIE:
;00000000  	0   NOP
;00000001	1	SWITCH
;00000011	3	BUTTON
;00000101	5	BUTTON ON
;00000111	7	BUTTON OFF
;00100001	33  ON/OFF
;00001000	8	RELAY
;00010000	16	COIL1
;00010010	18	COIL2
;01000000	64	LED
;01000010	66	LED BLINKING
;10000000	128	SERVO

		MOVLW 0xB0		        	;is opcode OPC_SW_REQ (B0)
		CPFSEQ LN_BYTE_RECEIVE1
		GOTO DLN_NEXT1

 		BTFSS LN_BYTE_RECEIVE3,4    ; Is ReceiveByte bit 4= 0 (ON bit) dan doe niets
        RETURN

		LFSR 0,TBL_ADR1L	    	;Laad startadres TBL_ADR1L in File Select Register 0
		LFSR 2,TBL_ADR1H	    	;Laad startadres TBL_ADR1H in File Select Register 2
		CLRF POORT_TELLER

DLN_LUS1

		MOVF LN_BYTE_RECEIVE2,W  	;ga na of adres poort = adres in opcode
		CPFSEQ INDF0
		BRA DLN_SPR10
		MOVF LN_BYTE_RECEIVE3,W
		ANDLW b'00001111'
		CPFSEQ INDF2
		BRA DLN_SPR10

		MOVLW HIGH TBL_FUNCTIE
        MOVWF FSR1H
        MOVLW LOW TBL_FUNCTIE
		ADDWF POORT_TELLER,W
        MOVWF FSR1L
        MOVF INDF1,W
        MOVWF POORT_FUNCTIE

        ;**********************************************************
        ;************ OPCODE B0: LED, RELAY, SERVO ****************
        ;**********************************************************

        BTFSC POORT_FUNCTIE,7     ;Is poortfunctie Servo
        BRA DLN_SPR20
        BTFSC POORT_FUNCTIE,6     ; OF is poortfunctie LED
        BRA DLN_SPR20
        BTFSS POORT_FUNCTIE,3     ; OF is poortfunctie Relay
        BRA DLN_SPR25
DLN_SPR20       
        LFSR 1,TBL_BINAIR
        MOVF POORT_TELLER,W
        ADDWF FSR1L,F
        BCF INDF1,4              ; Dan Zet binair bit4 = 0 als DIR = '0' anders binair bit4 ='1'
        BTFSC LN_BYTE_RECEIVE3,5
        BSF INDF1,4
        MOVLW b'00000000'
        BTFSC LN_BYTE_RECEIVE3,5
        MOVLW b'00010010'
 ;       MOVWF INDF1
 ;       MOVF INDF1,W                ;    en opslaan in EEPROM
        MOVWF EEDATA
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        BTFSC EECON1,WR
        BRA $-2
        CALL COPY_NAAR_EEPROM
        BRA DLN_SPR10


        ;**********************************************************
        ;************ OPCODE B0: COIL1, COIL2 *********************
        ;**********************************************************

DLN_SPR25
        BTFSS POORT_FUNCTIE,4               ;Is poortfunctie SPOEL1 of SPOEL2
        BRA DLN_SPR110

        LFSR 1,TBL_BINAIR
        MOVF POORT_TELLER,W
        ADDWF FSR1L,F        

        BTFSC POORT_FUNCTIE,1               ;  Is Dit SPOEL1
        BRA DLN_SPR40

        INCF FSR0L,F                        ;  JA: (=SPOEL1)
        INCF FSR2L,F
		MOVF LN_BYTE_RECEIVE2,W         	;     Is adres poort+1 = adres opcode?
		CPFSEQ INDF0 
		BRA DLN_SPR35
		MOVF LN_BYTE_RECEIVE3,W
		ANDLW b'00001111'
		CPFSEQ INDF2
		BRA DLN_SPR35

        BCF INDF1,4
        BTFSC LN_BYTE_RECEIVE3,5            ;     JA: dan bij DIR ='1' SET Bit4 anders RESET Bit4 TBL_BINAIR
        BSF INDF1,4
        MOVF INDF1,W
        MOVWF EEDATA                        ;         en opslaan in EEPROM
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        BTFSC EECON1,WR
        BRA $-2
        CALL COPY_NAAR_EEPROM
        BRA DLN_SPR39      

DLN_SPR35
        BTFSS LN_BYTE_RECEIVE3,5            ;     NEE: als DIR '1' dan SET BIT4 TBL_BINAIR en RESET BIT4 (TBL_BINAIR+1)
        BRA DLN_SPR37

        BSF INDF1,4
        MOVF INDF1,W
        MOVWF EEDATA                        ;          en opslaan in EEPROM
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        BTFSC EECON1,WR
        BRA $-2
        CALL COPY_NAAR_EEPROM
        BRA DLN_SPR39

DLN_SPR37
        INCF FSR1L,F
        BCF INDF1,4
        MOVF INDF1,W
        MOVWF EEDATA                        ;          en opslaan in EEPROM
        MOVF POORT_TELLER,W
        ADDLW .129
        MOVWF EEADR
        CLRF EEADRH       
        BTFSC EECON1,WR
        BRA $-2
        CALL COPY_NAAR_EEPROM
;        DECF FSR1L,F
 
DLN_SPR39
        DECF FSR0L,F                       
        DECF FSR2L,F
        BRA DLN_SPR10
   
        ;**********************************************************

DLN_SPR40   
        DECF FSR0L,F                        ;   NEE: (=SPOEL2)
        DECF FSR2L,F
		MOVF LN_BYTE_RECEIVE2,W         	;     Is adres poort-1 = adres opcode?
		CPFSEQ INDF0
		BRA DLN_SPR45
		MOVF LN_BYTE_RECEIVE3,W
		ANDLW b'00001111'
		CPFSEQ INDF2
		BRA DLN_SPR45   

        BSF INDF1,4
        BTFSC LN_BYTE_RECEIVE3,5            ;     JA: dan bij DIR ='1' SET Bit4 anders RESET Bit4 TBL_BINAIR
        BCF INDF1,4
        MOVF INDF1,W
        MOVWF EEDATA                        ;         en opslaan in EEPROM
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        BTFSC EECON1,WR
        BRA $-2
        CALL COPY_NAAR_EEPROM
        BRA DLN_SPR49      

DLN_SPR45
        BTFSS LN_BYTE_RECEIVE3,5            ;     NEE: als DIR '1' dan SET BIT4 TBL_BINAIR en RESET BIT4 (TBL_BINAIR-1)
        BRA DLN_SPR47

        BSF INDF1,4
        MOVF INDF1,W
        MOVWF EEDATA                        ;        en opslaan in EEPROM
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        BTFSC EECON1,WR
        BRA $-2
        CALL COPY_NAAR_EEPROM
        BRA DLN_SPR49

DLN_SPR47
        DECF FSR1L,F
        BCF INDF1,4
        MOVF INDF1,W
        MOVWF EEDATA                        ;      en opslaan in EEPROM
        MOVF POORT_TELLER,W
        ADDLW .127
        MOVWF EEADR
        CLRF EEADRH       
        BTFSC EECON1,WR
        BRA $-2
        CALL COPY_NAAR_EEPROM
;        INCF FSR1L,F
 
DLN_SPR49
        INCF FSR0L,F                       
        INCF FSR2L,F
        BRA DLN_SPR10

        ;**********************************************************
        ;************ OPCODE B0: BUTTON ***************************
        ;**********************************************************

;3/3/2017: Bijgevoegd zodat andere BUTTON ingangen met hetzelfde adres elkaar kunnen volgen

DLN_SPR110

        MOVLW b'00000011'
        CPFSEQ POORT_FUNCTIE           
        BRA DLN_SPR10

        LFSR 1,TBL_BINAIR
        MOVF POORT_TELLER,W
        ADDWF FSR1,F
        BCF INDF1,3
        BTFSC LN_BYTE_RECEIVE3,5
        BSF INDF1,3
      
        ;**********************************************************

DLN_SPR10
    	INCF FSR0L,F,A			;Verhoog File Select Register
		INCF FSR2L,F,A
		INCF POORT_TELLER,F
        MOVLW 32
        CPFSEQ POORT_TELLER
		BRA DLN_LUS1
        RETURN



;***************************************************************************

;LN_BYTE_RECEIVE1=  OPC_PEER_XFER  (E5)
;LN_BYTE_RECEIVE2=  h10
;LN_BYTE_RECEIVE3=  BRON 
;LN_BYTE_RECEIVE4=  DEVICENR
;LN_BYTE_RECEIVE5=  POORTNR
;LN_BYTE_RECEIVE6=  Bit7 van D1-D4
;LN_BYTE_RECEIVE7=  D1: ADRESNR
;LN_BYTE_RECEIVE8=  D2: FUNCTIE
;LN_BYTE_RECEIVE9=  D3: CONST1
;LN_BYTE_RECEIVE10= D4: CONST2
;LN_BYTE_RECEIVE11= Bit7 van D5-D8
;LN_BYTE_RECEIVE12= D5: CONST3
;LN_BYTE_RECEIVE13= D6: BINAIR
;LN_BYTE_RECEIVE14= D7: ADRESLOW
;LN_BYTE_RECEIVE15= D8: ADRESHIGH

;BRON= h10 --> master wil gegevens opvragen van DEVICENR,POORTNUMMER,ADRESNR
;BRON= h11 --> master wil gegevens updaten van DEVICENR,POORTNUMMER,ADRESNR
;BRON= h12 --> DEVICE stuurt de gevraagde gegevens door

DLN_NEXT1
		MOVLW 0xE5		        	;is opcode OPC_PEER_XFER  (E5)
		CPFSEQ LN_BYTE_RECEIVE1
		GOTO DLN_NEXT2

		MOVF (LOW LN_RECEIVE_BUFFER)+3,W,BANKED
		MOVWF LN_BYTE_RECEIVE4
		MOVF (LOW LN_RECEIVE_BUFFER)+4,W,BANKED
		MOVWF LN_BYTE_RECEIVE5
		MOVF (LOW LN_RECEIVE_BUFFER)+5,W,BANKED
		MOVWF LN_BYTE_RECEIVE6
		MOVF (LOW LN_RECEIVE_BUFFER)+6,W,BANKED
		MOVWF LN_BYTE_RECEIVE7
		MOVF (LOW LN_RECEIVE_BUFFER)+7,W,BANKED
		MOVWF LN_BYTE_RECEIVE8
		MOVF (LOW LN_RECEIVE_BUFFER)+8,W,BANKED
		MOVWF LN_BYTE_RECEIVE9
		MOVF (LOW LN_RECEIVE_BUFFER)+9,W,BANKED
		MOVWF LN_BYTE_RECEIVE10
		MOVF (LOW LN_RECEIVE_BUFFER)+10,W,BANKED
		MOVWF LN_BYTE_RECEIVE11
		MOVF (LOW LN_RECEIVE_BUFFER)+11,W,BANKED
		MOVWF LN_BYTE_RECEIVE12
		MOVF (LOW LN_RECEIVE_BUFFER)+12,W,BANKED
		MOVWF LN_BYTE_RECEIVE13
		MOVF (LOW LN_RECEIVE_BUFFER)+13,W,BANKED
		MOVWF LN_BYTE_RECEIVE14
		MOVF (LOW LN_RECEIVE_BUFFER)+14,W,BANKED
		MOVWF LN_BYTE_RECEIVE15

		MOVFF LN_BYTE_RECEIVE3,BRON		;adres master en wat er met data moet gebeuren
		MOVFF LN_BYTE_RECEIVE4,DEVICENR         ;Nummer IC  
		MOVFF LN_BYTE_RECEIVE5,POORTNR          ;poort1-32 voor ingangen poort33-   instellingen DV Device Variabelen (40 stuks mogelijk)
		MOVFF LN_BYTE_RECEIVE7,ADRESNR          ;adresnummer 1-12
		MOVFF LN_BYTE_RECEIVE8,FUNCTIE
		MOVFF LN_BYTE_RECEIVE9,CONST1
		MOVFF LN_BYTE_RECEIVE10,CONST2
		MOVFF LN_BYTE_RECEIVE12,CONST3
		MOVFF LN_BYTE_RECEIVE13,BINAIR
		MOVFF LN_BYTE_RECEIVE14,ADRESLOW	;adresbyte LOW van ADRESNR
		MOVFF LN_BYTE_RECEIVE15,ADRESHIGH       ;adresbyte HIGH van ADRESNR
		BTFSC LN_BYTE_RECEIVE6,0
		BSF ADRESNR,7
		BTFSC LN_BYTE_RECEIVE6,1
		BSF FUNCTIE,7
		BTFSC LN_BYTE_RECEIVE6,2
		BSF CONST1,7
		BTFSC LN_BYTE_RECEIVE6,3
		BSF CONST2,7
		BTFSC LN_BYTE_RECEIVE11,0
		BSF CONST3,7
		BTFSC LN_BYTE_RECEIVE11,1
		BSF BINAIR,7
		BTFSC LN_BYTE_RECEIVE11,2
		BSF ADRESLOW,7
		BTFSC LN_BYTE_RECEIVE11,3
		BSF ADRESHIGH,7
        
        MOVF DEVICENR,F             ;is ontvangen devicenummer = 0 dan broadcast (elk device luisterd)
        BZ DLN_NEXT22
		MOVF DEVICENUMMER,W			;is DEVICENR ontvangen gelijk aan devicenummer zoals opgeslagen in EEPROM?
		CPFSEQ DEVICENR
		RETURN

DLN_NEXT22

		MOVLW 0x10				;JA: is BRON = h10 dan gevraagde data terug zenden naar Master
		CPFSEQ BRON
		BRA DLN_SPR50


		CLRF TE_VERZENDEN_BYTE6
		CLRF TE_VERZENDEN_BYTE11

		MOVLW 0xE5				;     eerste te verzenden byte is opcode OPC_PEER_XFER  (E5)
		MOVWF TE_VERZENDEN_BYTE1

		MOVLW 0x10				;     tweede byte lengte datastroom =h10
		MOVWF TE_VERZENDEN_BYTE2

		MOVLW 0x12				;     derde byte ;BRON = h12
		MOVWF TE_VERZENDEN_BYTE3

		MOVF DEVICENR,W
		MOVWF TE_VERZENDEN_BYTE4

		MOVF POORTNR,W
		MOVWF TE_VERZENDEN_BYTE5	

		MOVF ADRESNR,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,0
		MOVWF TE_VERZENDEN_BYTE7

		MOVLW .30				;     als poortnummer minder of gelijk is aan 30 dan stuur poortgegevens
		CPFSLT POORTNR
		BRA DLN_SPR60
		DECF POORTNR,F

		LFSR 0, TBL_FUNCTIE
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,1
		MOVWF TE_VERZENDEN_BYTE8

		LFSR 0, TBL_CONST1
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,2
		MOVWF TE_VERZENDEN_BYTE9

		LFSR 0, TBL_CONST2
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,3
		MOVWF TE_VERZENDEN_BYTE10

		LFSR 0, TBL_CONST3
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,0
		MOVWF TE_VERZENDEN_BYTE12

		LFSR 0, TBL_BINAIR
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,1
		MOVWF TE_VERZENDEN_BYTE13

		DECF ADRESNR,W	
		MULLW 0x40
		MOVF PRODL,W
		ADDWF POORTNR,W
		MOVWF FSR0L
		MOVF PRODH,W
		ADDLW 3
		MOVWF FSR0H
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,2
		MOVWF TE_VERZENDEN_BYTE14

		MOVLW 0x20
		ADDWF FSR0L,F
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,3
		MOVWF TE_VERZENDEN_BYTE15

        MOVLW 16
        movwf AANTAL_BYTES
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
		RETURN


				
DLN_SPR60	
		LFSR 0,TBL_INSTELLINGEN
		DECF POORTNR,W
		ADDWF FSR0L,F
		MOVF INDF0,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,1
		MOVWF TE_VERZENDEN_BYTE8
		CLRF TE_VERZENDEN_BYTE9
		CLRF TE_VERZENDEN_BYTE10
		CLRF TE_VERZENDEN_BYTE12
		CLRF TE_VERZENDEN_BYTE13
		CLRF TE_VERZENDEN_BYTE14
		CLRF TE_VERZENDEN_BYTE15
	
        MOVLW 16
        movwf AANTAL_BYTES	
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
		RETURN



DLN_SPR50       ;NOG TE BEKIJKEN 23/JUL 2017


		MOVLW 0x12				;JA: is BRON = h12 en device is master dan verwerk gegevens naar Display
		CPFSEQ BRON
		BRA DLN_SPR55
;		BTFSS DISPLAY_ACTIEF
		BRA DLN_SPR60
;		BSF NIEUW_DISPLAY_DATA
		RETURN


DLN_SPR55
		MOVLW 0x11				;JA: is BRON = h11 dan data updaten in device
		CPFSEQ BRON
		RETURN

		MOVLW .32				;     als poortnummer minder of gelijk is aan 32 dan update poortgegevens
		CPFSLT POORTNR
		BRA DLN_SPR70
		DECF POORTNR,F
		
        CLRF EEADRH
        MOVF POORTNR,W
        MOVWF EEADR

		LFSR 0,TBL_FUNCTIE
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF FUNCTIE,W
		MOVWF INDF0
        MOVWF EEDATA
        CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_CONST1
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF CONST1,W
		MOVWF INDF0
        MOVWF EEDATA
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_CONST2
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF CONST2,W
		MOVWF INDF0
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_CONST3
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF CONST3,W
		MOVWF INDF0
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_BINAIR
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF BINAIR,W
		MOVWF INDF0
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		DECF ADRESNR,W	
		MULLW 0x40
		MOVF PRODL,W
		ADDWF POORTNR,W
		MOVWF FSR0L
        MOVWF EEADR
		MOVF PRODH,W
		INCF WREG,W
        MOVWF EEADRH
        INCF WREG,W
		MOVWF FSR0H
		MOVF ADRESLOW,W
		MOVWF INDF0
        MOVWF EEDATA
        CALL COPY_NAAR_EEPROM        	

		MOVLW 0x20
		ADDWF FSR0L,F
        MOVLW 0x20
        ADDWF EEADR,F
		MOVF ADRESH
		MOVWF INDF0
        MOVWF EEDATA
        CALL COPY_NAAR_EEPROM

		RETURN

DLN_SPR70
		LFSR 0,TBL_INSTELLINGEN
		DECF POORTNR,W
		ADDWF FSR0L,F
        CLRF EEADRH
        MOVLW 0xE0
        ADDWF POORTNR,W
        MOVWF EEADR
        
		MOVF FUNCTIE
		MOVWF INDF0
        MOVWF EEDATA
        CALL COPY_NAAR_EEPROM
		RETURN

;***************************************************************************

DLN_NEXT2
	RETURN




			