
;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 15 juni 2019                     Versie LOCONET IO V6.0          *
;*                                                                         *
;* Filenaam: LOCONET_DECODEER_V6p0.INC                                     *
;*                                                                         *
;***************************************************************************

    #DEFINE SEND_ALL_INPUTS LN_FLAGS_2,0    ;after OPC_GPON send status all INPUTS


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

		MOVLW OPC_SW_REQ		        	;is opcode OPC_SW_REQ (B0)
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
        ;************ OPC_SW_REQ: LED, RELAY, SERVO ***************
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
           
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM

        IF STATUS_COUNTERS == 1
           INCREMENT_OUTPUT_ACTION_COUNTER
        ENDIF

        BRA DLN_SPR10

        ;**********************************************************
        ;************ OPC_SW_REQ: COIL1, COIL2 ********************
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

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA                
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM

        IF STATUS_COUNTERS == 1
           INCREMENT_OUTPUT_ACTION_COUNTER
        ENDIF

        BRA DLN_SPR39      

DLN_SPR35
        BTFSS LN_BYTE_RECEIVE3,5            ;     NEE: als DIR '1' dan SET BIT4 TBL_BINAIR en RESET BIT4 (TBL_BINAIR+1)
        BRA DLN_SPR37

        BSF INDF1,4
        MOVF INDF1,W

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA           
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM

        IF STATUS_COUNTERS == 1
           INCREMENT_OUTPUT_ACTION_COUNTER
        ENDIF

        BRA DLN_SPR39

DLN_SPR37
        INCF FSR1L,F
        BCF INDF1,4
        MOVF INDF1,W

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA              
        MOVF POORT_TELLER,W
        ADDLW .129
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM

        IF STATUS_COUNTERS == 1
           INCREMENT_OUTPUT_ACTION_COUNTER
        ENDIF
 
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

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA                
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM

        IF STATUS_COUNTERS == 1
           INCREMENT_OUTPUT_ACTION_COUNTER
        ENDIF

        BRA DLN_SPR49      

DLN_SPR45
        BTFSS LN_BYTE_RECEIVE3,5            ;     NEE: als DIR '1' dan SET BIT4 TBL_BINAIR en RESET BIT4 (TBL_BINAIR-1)
        BRA DLN_SPR47

        BSF INDF1,4
        MOVF INDF1,W

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA              
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM

        IF STATUS_COUNTERS == 1
           INCREMENT_OUTPUT_ACTION_COUNTER
        ENDIF

        BRA DLN_SPR49

DLN_SPR47
        DECF FSR1L,F
        BCF INDF1,4
        MOVF INDF1,W

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA      
        MOVF POORT_TELLER,W
        ADDLW .127
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM

        IF STATUS_COUNTERS == 1
           INCREMENT_OUTPUT_ACTION_COUNTER
        ENDIF
 
DLN_SPR49
        INCF FSR0L,F                       
        INCF FSR2L,F
        BRA DLN_SPR10


        ;**********************************************************
        ;************ OPC_SW_REQ: BUTTON **************************
        ;**********************************************************

;3/3/2017: Bijgevoegd zodat andere BUTTON ingangen met hetzelfde adres elkaar kunnen volgen

DLN_SPR110

        MOVLW FUNCTIE_BUTTON
        CPFSEQ POORT_FUNCTIE           
        BRA DLN_SPR10
        BTFSS LN_BYTE_RECEIVE3,4
        BRA DLN_SPR10

        LFSR 1,TBL_BINAIR
        MOVF POORT_TELLER,W
        ADDWF FSR1L,F
        BCF INDF1,3
        BTFSC LN_BYTE_RECEIVE3,5
        BSF INDF1,3

        MOVF INDF1,W                 ;15/07/2019 toegevoegd om positie drukknop te onthouden in EEPROM
        BTFSC EECON1,WR 
        BRA $-2
        MOVWF EEDATA              
        MOVF POORT_TELLER,W
        ADDLW .128
        MOVWF EEADR
        CLRF EEADRH       
        CALL COPY_NAAR_EEPROM        ;15/07/2019
      
        ;**********************************************************

DLN_SPR10
    	INCF FSR0L,F,A			;Verhoog File Select Register
		INCF FSR2L,F,A
		INCF POORT_TELLER,F
        MOVLW 30
        CPFSEQ POORT_TELLER
		BRA DLN_LUS1

        RETURN


;***************************************************************************

;LN_BYTE_RECEIVE1=  OPC_PEER_XFER  (E5)
;LN_BYTE_RECEIVE2=  h10
;LN_BYTE_RECEIVE3=  CODE
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


DLN_NEXT1
		MOVLW OPC_PEER_XFER		        	;is opcode OPC_PEER_XFER  (E5)
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

		MOVFF LN_BYTE_RECEIVE3,BRONCODE		    ;wat er met data moet gebeuren
		MOVFF LN_BYTE_RECEIVE4,DEVICENR         ;Nummer IC  
		MOVFF LN_BYTE_RECEIVE5,POORTNR          ;poort1-30 voor IC-ingangen; poort 31-34 logische functies
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

    IF DEBUGGER == 0
        BTFSC PROGRAM_NEW_DEVICE_ADRES         ;als ingang PORT_PROGRAM =0 dan update EEPROM nieuw Device adress
        GOTO DLN_NEXT21
        MOVF DEVICENR,w
        MOVWF DEVICENUMMER

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA
        CLRF EEADRH
        MOVLW 0x1F
        MOVWF EEADR
        CALL COPY_NAAR_EEPROM
        GOTO DLN_NEXT22
    ENDIF

DLN_NEXT21        
        MOVF DEVICENR,F             ;is ontvangen devicenummer = 0 dan broadcast (elk device luisterd)
        BZ DLN_NEXT22
		MOVF DEVICENUMMER,W			;is DEVICENR ontvangen gelijk aan devicenummer zoals opgeslagen in EEPROM?
		CPFSEQ DEVICENR
		RETURN


DLN_NEXT22

		MOVLW CODE_UPLOAD				;CODE = UPLOAD DATA dan gevraagde data terug zenden naar Master
		CPFSEQ BRONCODE
		BRA DLN_SPR50


		CLRF TE_VERZENDEN_BYTE6
		CLRF TE_VERZENDEN_BYTE11

		MOVLW OPC_PEER_XFER				;     eerste te verzenden byte is opcode OPC_PEER_XFER  (E5)
		MOVWF TE_VERZENDEN_BYTE1

		MOVLW 0x10				;     tweede byte lengte datastroom =h10
		MOVWF TE_VERZENDEN_BYTE2

		MOVLW CODE_DATA				;     derde byte: CODE DATA data naar Master
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

		DECF POORTNR,F
		CLRF EEADRH

		MOVLW EEPROM_ADR_LOW_FUNCTION
		ADDWF POORTNR,W
		MOVWF EEADR
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,1
		MOVWF TE_VERZENDEN_BYTE8

		MOVLW EEPROM_ADR_LOW_CONST1
		ADDWF POORTNR,W
		MOVWF EEADR
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,2
		MOVWF TE_VERZENDEN_BYTE9

		MOVLW EEPROM_ADR_LOW_CONST2
		ADDWF POORTNR,W
		MOVWF EEADR
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,3
		MOVWF TE_VERZENDEN_BYTE10

		MOVLW EEPROM_ADR_LOW_CONST3
		ADDWF POORTNR,W
		MOVWF EEADR
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,0
		MOVWF TE_VERZENDEN_BYTE12

		MOVLW EEPROM_ADR_LOW_BINAIR
		ADDWF POORTNR,W
		MOVWF EEADR
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,1
		MOVWF TE_VERZENDEN_BYTE13

		DECF ADRESNR,W
		BNZ DLN_SPR62			;enkel ADR1L/H opvragen uit EEPROM 0xC0 en 0xD0

		MOVLW EEPROM_ADR_LOW_ADR1L
		ADDWF POORTNR,W
		MOVWF EEADR
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,2
		MOVWF TE_VERZENDEN_BYTE14

		MOVLW EEPROM_ADR_LOW_ADR1H
		ADDWF POORTNR,W
		MOVWF EEADR
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,3
		MOVWF TE_VERZENDEN_BYTE15


        MOVLW 16
        movwf AANTAL_BYTES
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
		RETURN


DLN_SPR62
		DECF ADRESNR,W	
		MULLW 0x40
		MOVF PRODL,W
		ADDWF POORTNR,W
		MOVWF EEADR
		MOVF PRODH,W
		ADDLW 1
		MOVWF EEADRH
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,2
		MOVWF TE_VERZENDEN_BYTE14

		MOVLW 0x20
		ADDWF EEADR,F
		BSF EECON1,RD
		MOVF EEDATA,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,3
		MOVWF TE_VERZENDEN_BYTE15

        MOVLW 16
       	movwf AANTAL_BYTES
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
		RETURN

				
DLN_SPR50    

		MOVLW CODE_UPDATE				;data updaten in device
		CPFSEQ BRONCODE
		GOTO DLN_SPR70

		DECF POORTNR,F
		
        CLRF EEADRH
        MOVF POORTNR,W

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
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
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_CONST2
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF CONST2,W
		MOVWF INDF0
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_CONST3
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF CONST3,W
		MOVWF INDF0
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA
        MOVLW 0x20
        ADDWF EEADR,F
       	CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_BINAIR
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF BINAIR,W
		MOVWF INDF0
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		DECF ADRESNR,W
		BNZ DLN_SPR72

		LFSR 0,TBL_ADR1L
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF ADRESLOW,W
		MOVWF INDF0
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
       	MOVWF EEDATA
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		LFSR 0,TBL_ADR1H
		MOVF POORTNR,W
		ADDWF FSR0L,F
		MOVF ADRESHIGH,W
		MOVWF INDF0
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA
        MOVLW 0x20
        ADDWF EEADR,F
        CALL COPY_NAAR_EEPROM

		RETURN

DLN_SPR72
		DECF ADRESNR,W	
		MULLW 0x40
		MOVF PRODL,W
		ADDWF POORTNR,W
		MOVWF FSR0L
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEADR
		MOVF PRODH,W
		ADDLW 1 
        MOVWF EEADRH
		MOVF PRODH,W
        ADDLW HIGH TBL_ADR1L
		MOVWF FSR0H
		MOVF ADRESLOW,W
		MOVWF INDF0
       	MOVWF EEDATA
        CALL COPY_NAAR_EEPROM        	

		MOVLW 0x20
		ADDWF FSR0L,F
        MOVLW 0x20
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        ADDWF EEADR,F
		MOVF ADRESHIGH,W
		MOVWF INDF0
        MOVWF EEDATA
       	CALL COPY_NAAR_EEPROM

		RETURN

.
DLN_SPR70

        MOVLW CODE_RESET            ;RESET device
        CPFSEQ BRONCODE
        GOTO DLN_SPR85


        MOVF ADRESNR,W               ;voor de zekerheid moet data overeenkomen met verzonden data
        XORLW 0x20
        BTFSS STATUS,Z
        RETURN
        MOVF FUNCTIE ,W
        XORLW 0x52
        BTFSS STATUS,Z
        RETURN
        MOVF CONST1,W
        XORLW 0x45
        BTFSS STATUS,Z
        RETURN
        MOVF CONST2,W
        XORLW 0x53
        BTFSS STATUS,Z
        RETURN
        MOVF CONST3,W
        XORLW 0x45
        BTFSS STATUS,Z
        RETURN
        MOVF BINAIR,W
        XORLW 0x54
        BTFSS STATUS,Z
        RETURN
        MOVF ADRESLOW,W
        XORLW 0x21
        BTFSS STATUS,Z
        RETURN
        MOVF ADRESHIGH,W
        XORLW 0x20
        BTFSS STATUS,Z
        RETURN

        RESET


DLN_SPR85

        MOVLW CODE_CLEAR            ;CLEAR device
        CPFSEQ BRONCODE
        GOTO DLN_SPR95


        MOVF ADRESNR,W               ;voor de zekerheid moet data overeenkomen met verzonden data
        XORLW 0x43     ;C
        BTFSS STATUS,Z
        RETURN
        MOVF FUNCTIE ,W
        XORLW 0x4C     ;L
        BTFSS STATUS,Z
        RETURN
        MOVF CONST1,W
        XORLW 0x45     ;E
        BTFSS STATUS,Z
        RETURN
        MOVF CONST2,W
        XORLW 0x41     ;A
        BTFSS STATUS,Z
        RETURN
        MOVF CONST3,W
        XORLW 0x52     ;R
        BTFSS STATUS,Z
        RETURN
        MOVF BINAIR,W
        XORLW 0x20
        BTFSS STATUS,Z
        RETURN
        MOVF ADRESLOW,W
        XORLW 0x20
        BTFSS STATUS,Z
        RETURN
        MOVF ADRESHIGH,W
        XORLW 0x20
        BTFSS STATUS,Z
        RETURN

        CLRF EEADRH 
        CLRF EEADR  
        CLRF EEDATA  
        CLRF HULPTEL  

DLN_SPR86
        CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR             
        BRA $-2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA DLN_SPR86	
        INCF EEADRH,F 

DLN_SPR87
        CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR             
        BRA $-2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA DLN_SPR87
        INCF EEADRH,F 

DLN_SPR88
        CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR             
        BRA $-2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA DLN_SPR88
        INCF EEADRH,F 

DLN_SPR89
        CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR             
        BRA $-2
        INCF EEADR,F  
        DECFSZ HULPTEL,F  
        BRA DLN_SPR89	



        BTFSC EECON1,WR              ;   Devicenr terug opslaan in EEPROM
        BRA $-2
        MOVF DEVICENR,w
        MOVWF EEDATA
        CLRF EEADRH
        MOVLW 0x1F
        MOVWF EEADR
        CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR             
        BRA $-2

        RESET


DLN_SPR95

        MOVLW CODE_RESET_C            ;RESET device Counters
        CPFSEQ BRONCODE
        GOTO DLN_SPR96


        MOVF ADRESNR,W               ;voor de zekerheid moet data overeenkomen met verzonden data
        XORLW 0x20
        BTFSS STATUS,Z
        RETURN
        MOVF FUNCTIE ,W
        XORLW 0x52
        BTFSS STATUS,Z
        RETURN
        MOVF CONST1,W
        XORLW 0x45
        BTFSS STATUS,Z
        RETURN
        MOVF CONST2,W
        XORLW 0x53
        BTFSS STATUS,Z
        RETURN
        MOVF CONST3,W
        XORLW 0x45
        BTFSS STATUS,Z
        RETURN
        MOVF BINAIR,W
        XORLW 0x54
        BTFSS STATUS,Z
        RETURN
        MOVF ADRESLOW,W
        XORLW 0x21
        BTFSS STATUS,Z
        RETURN
        MOVF ADRESHIGH,W
        XORLW 0x20
        BTFSS STATUS,Z
        RETURN

        CALL INIT_DEVICE_COUNTERS       
        RETURN

DLN_SPR96
        MOVLW CODE_COUNTERS               ;sent device counters
        CPFSEQ BRONCODE
        RETURN
        GOTO SENT_COUNTERS


;***************************************************************************

DLN_NEXT2
		MOVLW OPC_GPON		        	;is opcode OPC_GPON
		CPFSEQ LN_BYTE_RECEIVE1
		GOTO DLN_NEXT3
        BSF SEND_ALL_INPUTS
        CALL TEST_INGANG
        BCF SEND_ALL_INPUTS
        RETURN      

DLN_NEXT3
	RETURN




			



			