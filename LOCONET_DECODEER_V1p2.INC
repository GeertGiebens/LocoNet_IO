
;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 10 jan 2019                      Versie LOCONET IO V1.2          *
;*                                                                         *
;* Filenaam: LOCONET_DECODEER_V1p2.INC                                     *
;*                                                                         *
;***************************************************************************

    #DEFINE TEST_LN_SNELHEID 1   ;gebruikt als test om niet te veel data te schrijven naar EEPROM 0=test

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
      
        ;**********************************************************

DLN_SPR10
    	INCF FSR0L,F,A			;Verhoog File Select Register
		INCF FSR2L,F,A
		INCF POORT_TELLER,F
        MOVLW 30
        CPFSEQ POORT_TELLER
		BRA DLN_LUS1

        ;**********************************************************

;Logische poorten verwerking:

;Functie:
;NOP    00000000  00
;AND    00000010  02
;NAND   00000011  03
;OR     00000100  04
;NOR    00000101  05 
;EXOR   00001000  08
;NEXOR  00001001  09
;FF     00010000  16

;Binair:
;0XX0XXXX
; || ||||
; || |||X=0 VORIGE DIR IN1=0;   =1 DIR IN1=1
; || ||X=0  VORIGE DIR IN2=0;   =1 DIR IN2=1
; || |X=0 NIEUWE DIR IN1=0;   =1 DIR IN1=1
; || X=0  NIEUWE DIR IN2=0;   =1 DIR IN2=1
; |X=toestand Flip Flop (indien van toepassing)
; X=1 verandering van toestand

;EEPROM Geheugen:
;0x2E0
;Logische functie; Binair; Functie; ADR_LB_IN1; ADR_HB_IN1; ADR_LB_IN2; ADR_HB_IN2; ADR_LB_OUT; ADR_HB_OUT
;31                 0x2E0   0x2E1     0x2E2       0x2E3       0x2E4       0x2E5       0x2E6       0x2E7
;32                 0x2E8   0x2E9     0x2EA       0x2EB       0x2EC       0x2ED       0x2EE       0x2EF
;33                 0x2F0   0x2F1     0x2F2       0x2F3       0x2F4       0x2F5       0x2F6       0x2F7
;34                 0x2F8   0x2F9     0x2FA       0x2FB       0x2FC       0x2FD       0x2FE       0x2FF 

      ;**********************************************************

VERWERK_LOGISCHE_POORT MACRO VP_BINAIR, VP_FUNCTIE, VP_A_LB_IN1, VP_A_HB_IN1, VP_A_LB_IN2, VP_A_HB_IN2, VP_A_LB_OUT, VP_A_HB_OUT
    
        LOCAL VP_SPR10,VP_SPREND,VP_SPR20,VP_SPR30, VP_SPR40,VP_SPR21,VP_SPR22,VP_SPR23,VP_SPR24, VP_SPR50

        MOVF VP_FUNCTIE,w
        BZ VP_SPREND

		MOVF LN_BYTE_RECEIVE2,W  	;ga na of adres poort = adres in opcode Adres IN1
		CPFSEQ VP_A_LB_IN1
		BRA VP_SPR10
		MOVF LN_BYTE_RECEIVE3,W
		ANDLW b'00001111'
		CPFSEQ VP_A_HB_IN1
		BRA VP_SPR10

        BCF VP_BINAIR,2
        BTFSC LN_BYTE_RECEIVE3,5
        BSF VP_BINAIR,2        
     
VP_SPR10
		MOVF LN_BYTE_RECEIVE2,W  	;ga na of adres poort = adres in opcode Adres IN2 
		CPFSEQ VP_A_LB_IN2
		BRA VP_SPR20
		MOVF LN_BYTE_RECEIVE3,W
		ANDLW b'00001111'
		CPFSEQ VP_A_HB_IN2
		BRA VP_SPR20 

        BCF VP_BINAIR,3
        BTFSC LN_BYTE_RECEIVE3,5
        BSF VP_BINAIR,3

VP_SPR20

        BTFSS VP_BINAIR,2           ;is er verandering in toestand IN1
        BRA VP_SPR21
        BTFSC VP_BINAIR,0
        BRA VP_SPR22
        BSF VP_BINAIR,0
        BSF VP_BINAIR,5
        BSF VP_BINAIR,6
        BRA VP_SPR22
VP_SPR21
        BTFSS VP_BINAIR,0
        BRA VP_SPR22
        BCF VP_BINAIR,0
        BSF VP_BINAIR,6

VP_SPR22
        BTFSS VP_BINAIR,3           ;is er verandering in toestand IN2
        BRA VP_SPR23
        BTFSC VP_BINAIR,1
        BRA VP_SPR24
        BSF VP_BINAIR,1
        BCF VP_BINAIR,5
        BSF VP_BINAIR,6
        BRA VP_SPR24
VP_SPR23
        BTFSS VP_BINAIR,1
        BRA VP_SPR24
        BCF VP_BINAIR,1
        BSF VP_BINAIR,6

VP_SPR24
        BTFSS VP_BINAIR,6           ;Is er een verandering dan:       
        BRA VP_SPREND
        BCF VP_BINAIR,6

        MOVF VP_BINAIR,W

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
        MOVWF EEDATA              
        MOVLW LOW VP_BINAIR
        MOVWF EEADR
        CLRF EEADRH
    IF TEST_LN_SNELHEID ==1      
        CALL COPY_NAAR_EEPROM
    ENDIF
        MOVLW 0xB0
        MOVWF TE_VERZENDEN_BYTE1
        MOVLW 4
        MOVWF AANTAL_BYTES
        MOVF VP_A_LB_OUT,W
        MOVWF TE_VERZENDEN_BYTE2
        MOVF VP_A_HB_OUT,W
        MOVWF TE_VERZENDEN_BYTE3
        MOVLW b'00010000'
        IORWF TE_VERZENDEN_BYTE3,F
        

        BTFSS VP_FUNCTIE,1          ;fuctie AND/NAND
        BRA VP_SPR30

        BTFSS VP_BINAIR,0
        BRA $+6
        BTFSC VP_BINAIR,1
        BSF TE_VERZENDEN_BYTE3,5

        BTFSC VP_FUNCTIE,0
        BTG TE_VERZENDEN_BYTE3,5
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        BRA VP_SPREND
        

VP_SPR30
        BTFSS VP_FUNCTIE,2          ;fuctie OR/NOR
        BRA VP_SPR40

        BTFSC VP_BINAIR,0
        BSF TE_VERZENDEN_BYTE3,5
        BTFSC VP_BINAIR,1
        BSF TE_VERZENDEN_BYTE3,5

        BTFSC VP_FUNCTIE,0
        BTG TE_VERZENDEN_BYTE3,5
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        BRA VP_SPREND

VP_SPR40
        BTFSS VP_FUNCTIE,3          ;fuctie EXOR/NEXOR
        BRA VP_SPR50

        BTFSC VP_BINAIR,0
        BSF TE_VERZENDEN_BYTE3,5
        BTFSC VP_BINAIR,1
        BSF TE_VERZENDEN_BYTE3,5
        
        BTFSS VP_BINAIR,0
        BRA $+6
        BTFSC VP_BINAIR,1
        BCF TE_VERZENDEN_BYTE3,5

        BTFSC VP_FUNCTIE,0
        BTG TE_VERZENDEN_BYTE3,5
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
        BRA VP_SPREND

VP_SPR50
        BTFSC VP_BINAIR,5           ;functie Flip Flop
        BSF TE_VERZENDEN_BYTE3,5
        CALL TE_VERZENDEN_BYTES_IN_BUFFER
              

VP_SPREND
        NOP

        ENDM

        ;**********************************************************

        MOVLB HIGH TBL_Logische_Functies
        VERWERK_LOGISCHE_POORT LF1ADR, LF1ADR+1, LF1ADR+2, LF1ADR+3, LF1ADR+4, LF1ADR+5, LF1ADR+6, LF1ADR+7
        VERWERK_LOGISCHE_POORT LF2ADR, LF2ADR+1, LF2ADR+2, LF2ADR+3, LF2ADR+4, LF2ADR+5, LF2ADR+6, LF2ADR+7
        VERWERK_LOGISCHE_POORT LF3ADR, LF3ADR+1, LF3ADR+2, LF3ADR+3, LF3ADR+4, LF3ADR+5, LF3ADR+6, LF3ADR+7
        VERWERK_LOGISCHE_POORT LF4ADR, LF4ADR+1, LF4ADR+2, LF4ADR+3, LF4ADR+4, LF4ADR+5, LF4ADR+6, LF4ADR+7                       

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
		MOVLW CODE_UPLOAD_L				; gevraagde data logische poorten terug zenden naar Master
		CPFSEQ BRONCODE
		BRA DLN_SPR55


		CLRF TE_VERZENDEN_BYTE6
		CLRF TE_VERZENDEN_BYTE11

		MOVLW OPC_PEER_XFER				;     eerste te verzenden byte is opcode OPC_PEER_XFER  (E5)
		MOVWF TE_VERZENDEN_BYTE1

		MOVLW 0x10				;     tweede byte lengte datastroom =h10
		MOVWF TE_VERZENDEN_BYTE2

		MOVLW CODE_DATA_L
		MOVWF TE_VERZENDEN_BYTE3

		MOVF DEVICENR,W
		MOVWF TE_VERZENDEN_BYTE4

		MOVF POORTNR,W
		MOVWF TE_VERZENDEN_BYTE5 

        MOVLB HIGH TBL_FUNCTIE
        MOVLW 16
        movwf AANTAL_BYTES

        MOVLW 31
        CPFSEQ POORTNR
        BRA DLN_SPR100
        MOVF 0xE0,W,BANKED
        MOVWF TE_VERZENDEN_BYTE7
        MOVF 0xE1,W,BANKED
        MOVWF TE_VERZENDEN_BYTE8
        MOVF 0xE2,W,BANKED
        MOVWF TE_VERZENDEN_BYTE9
        MOVF 0xE3,W,BANKED
        MOVWF TE_VERZENDEN_BYTE10
        MOVF 0xE4,W,BANKED
        MOVWF TE_VERZENDEN_BYTE12
        MOVF 0xE5,W,BANKED
        MOVWF TE_VERZENDEN_BYTE13
        MOVF 0xE6,W,BANKED
        MOVWF TE_VERZENDEN_BYTE14
        MOVF 0xE7,W,BANKED
        MOVWF TE_VERZENDEN_BYTE15
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
        RETURN

DLN_SPR100
        MOVLW 32
        CPFSEQ POORTNR
        BRA DLN_SPR115
        MOVF 0xE8,W,BANKED
        MOVWF TE_VERZENDEN_BYTE7
        MOVF 0xE9,W,BANKED
        MOVWF TE_VERZENDEN_BYTE8
        MOVF 0xEA,W,BANKED
        MOVWF TE_VERZENDEN_BYTE9
        MOVF 0xEB,W,BANKED
        MOVWF TE_VERZENDEN_BYTE10
        MOVF 0xEC,W,BANKED
        MOVWF TE_VERZENDEN_BYTE12
        MOVF 0xED,W,BANKED
        MOVWF TE_VERZENDEN_BYTE13
        MOVF 0xEE,W,BANKED
        MOVWF TE_VERZENDEN_BYTE14
        MOVF 0xEF,W,BANKED
        MOVWF TE_VERZENDEN_BYTE15
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
        RETURN

DLN_SPR115
        MOVLW 33
        CPFSEQ POORTNR
        BRA DLN_SPR120
        MOVF 0xF0,W,BANKED
        MOVWF TE_VERZENDEN_BYTE7
        MOVF 0xF1,W,BANKED
        MOVWF TE_VERZENDEN_BYTE8
        MOVF 0xF2,W,BANKED
        MOVWF TE_VERZENDEN_BYTE9
        MOVF 0xF3,W,BANKED
        MOVWF TE_VERZENDEN_BYTE10
        MOVF 0xF4,W,BANKED
        MOVWF TE_VERZENDEN_BYTE12
        MOVF 0xF5,W,BANKED
        MOVWF TE_VERZENDEN_BYTE13
        MOVF 0xF6,W,BANKED
        MOVWF TE_VERZENDEN_BYTE14
        MOVF 0xF7,W,BANKED
        MOVWF TE_VERZENDEN_BYTE15
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
        RETURN

DLN_SPR120 ;34
        MOVF 0xF8,W,BANKED
        MOVWF TE_VERZENDEN_BYTE7
        MOVF 0xF9,W,BANKED
        MOVWF TE_VERZENDEN_BYTE8
        MOVF 0xFA,W,BANKED
        MOVWF TE_VERZENDEN_BYTE9
        MOVF 0xFB,W,BANKED
        MOVWF TE_VERZENDEN_BYTE10
        MOVF 0xFC,W,BANKED
        MOVWF TE_VERZENDEN_BYTE12
        MOVF 0xFD,W,BANKED
        MOVWF TE_VERZENDEN_BYTE13
        MOVF 0xFE,W,BANKED
        MOVWF TE_VERZENDEN_BYTE14
        MOVF 0xFF,W,BANKED
        MOVWF TE_VERZENDEN_BYTE15
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
        RETURN
        


DLN_SPR55
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

DLN_SPR70
        MOVLW CODE_UPDATE_L                  ;update logische poort gegevens
        CPFSEQ BRONCODE
        GOTO DLN_SPR80

        MOVLB HIGH TBL_FUNCTIE
        CLRF EEADRH

        MOVLW 31
        CPFSEQ POORTNR
        BRA DLN_SPR150

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE0
        MOVWF EEADR
        MOVF ADRESNR,W
        MOVWF EEDATA
        MOVWF 0xE0,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE1
        MOVWF EEADR
        MOVF FUNCTIE,W
        MOVWF EEDATA
        MOVWF 0xE1,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE2
        MOVWF EEADR
        MOVF CONST1,W
        MOVWF EEDATA
        MOVWF 0xE2,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE3
        MOVWF EEADR
        MOVF CONST2,W
        MOVWF EEDATA
        MOVWF 0xE3,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE4
        MOVWF EEADR
        MOVF CONST3,W
        MOVWF EEDATA
        MOVWF 0xE4,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE5
        MOVWF EEADR
        MOVF BINAIR,W
        MOVWF EEDATA
        MOVWF 0xE5,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE6
        MOVWF EEADR
        MOVF ADRESLOW,W
        MOVWF EEDATA
        MOVWF 0xE6,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE7
        MOVWF EEADR
        MOVF ADRESHIGH,W
        MOVWF EEDATA
        MOVWF 0xE7,BANKED
       	CALL COPY_NAAR_EEPROM        
        RETURN

DLN_SPR150
        MOVLW 32
        CPFSEQ POORTNR
        BRA DLN_SPR160
        
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE8
        MOVWF EEADR
        MOVF ADRESNR,W
        MOVWF EEDATA
        MOVWF 0xE8,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xE9
        MOVWF EEADR
        MOVF FUNCTIE,W
        MOVWF EEDATA
        MOVWF 0xE9,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xEA
        MOVWF EEADR
        MOVF CONST1,W
        MOVWF EEDATA
        MOVWF 0xEA,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xEB
        MOVWF EEADR
        MOVF CONST2,W
        MOVWF EEDATA
        MOVWF 0xEB,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xEC
        MOVWF EEADR
        MOVF CONST3,W
        MOVWF EEDATA
        MOVWF 0xEC,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xED
        MOVWF EEADR
        MOVF BINAIR,W
        MOVWF EEDATA
        MOVWF 0xED,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xEE
        MOVWF EEADR
        MOVF ADRESLOW,W
        MOVWF EEDATA
        MOVWF 0xEE,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xEF
        MOVWF EEADR
        MOVF ADRESHIGH,W
        MOVWF EEDATA
        MOVWF 0xEF,BANKED
       	CALL COPY_NAAR_EEPROM        
        RETURN

DLN_SPR160
        MOVLW 33
        CPFSEQ POORTNR
        BRA DLN_SPR170
        
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF0
        MOVWF EEADR
        MOVF ADRESNR,W
        MOVWF EEDATA
        MOVWF 0xF0,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF1
        MOVWF EEADR
        MOVF FUNCTIE,W
        MOVWF EEDATA
        MOVWF 0xF1,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF2
        MOVWF EEADR
        MOVF CONST1,W
        MOVWF EEDATA
        MOVWF 0xF2,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF3
        MOVWF EEADR
        MOVF CONST2,W
        MOVWF EEDATA
        MOVWF 0xF3,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF4
        MOVWF EEADR
        MOVF CONST3,W
        MOVWF EEDATA
        MOVWF 0xF4,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF5
        MOVWF EEADR
        MOVF BINAIR,W
        MOVWF EEDATA
        MOVWF 0xF5,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF6
        MOVWF EEADR
        MOVF ADRESLOW,W
        MOVWF EEDATA
        MOVWF 0xF6,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF7
        MOVWF EEADR
        MOVF ADRESHIGH,W
        MOVWF EEDATA
        MOVWF 0xF7,BANKED
       	CALL COPY_NAAR_EEPROM        
        RETURN

DLN_SPR170

        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF8
        MOVWF EEADR
        MOVF ADRESNR,W
        MOVWF EEDATA
        MOVWF 0xF8,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xF9
        MOVWF EEADR
        MOVF FUNCTIE,W
        MOVWF EEDATA
        MOVWF 0xF9,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xFA
        MOVWF EEADR
        MOVF CONST1,W
        MOVWF EEDATA
        MOVWF 0xFA,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xFB
        MOVWF EEADR
        MOVF CONST2,W
        MOVWF EEDATA
        MOVWF 0xFB,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xFC
        MOVWF EEADR
        MOVF CONST3,W
        MOVWF EEDATA
        MOVWF 0xFC,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xFD
        MOVWF EEADR
        MOVF BINAIR,W
        MOVWF EEDATA
        MOVWF 0xFD,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xFE
        MOVWF EEADR
        MOVF ADRESLOW,W
        MOVWF EEDATA
        MOVWF 0xFE,BANKED
       	CALL COPY_NAAR_EEPROM
        BTFSC EECON1,WR              ;    en opslaan in EEPROM
        BRA $-2
		MOVLW 0xFF
        MOVWF EEADR
        MOVF ADRESHIGH,W
        MOVWF EEDATA
        MOVWF 0xFF,BANKED
       	CALL COPY_NAAR_EEPROM        
        RETURN

DLN_SPR80

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
	RETURN




			



			