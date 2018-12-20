;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 13 DEC 2018                      Versie LOCONET IO V1.2          *
;*                                                                         *
;* Filenaam: LOCONET_IO_DEVICE_COUNTERS_V1p2.INC                           *
;*                                                                         *
;***************************************************************************

;Counters to test the LocoNet communication bus.
;These counters can be requested with JMRI DecoderPro script tool: "LocoNet_IO_Status_V1p1.py" 

;VARIABELEN DEVICE COUNTERS

	cblock 0x50	;Device Counters
	
	LBCL	;LineBreak Counter byte High
	LBCH	;LineBreak Counter byte Low
	SBCL	;Sent Byte Counter byte Low
	SBCH	;Sent Byte Counter byte High
    SBCHH	;Sent Byte Counter byte High High
	RBCL	;Read Byte Counter byte Low
	RBCH	;Read Byte Counter byte High
    RBCHH	;Read Byte Counter byte High High
	RMCL	;Read Message Counter byte High
	RMCH	;Read Message Counter byte Low
	FECL	;Framing Error Counter byte Low
	FECH	;Framing Error Counter byte High
	CECL	;Checksum Error Counter byte Low
	CECH	;Checksum Error Counter byte High
	DSCL	;Device Seconds Counter byte Low
	DSCH	;Device Seconds Counter byte HIGH
	UAACL	;Unsuccessful Access Attempts Counter byte High
	UAACH	;Unsuccessful Access Attempts Counter byte Low
	SMCL	;Sent Message Counter byte Low
	SMCH	;Sent Message Counter byte High
    OACL    ;Output Actions Counter byte Low
    OACH    ;Output Actions Counter byte High
    RFCL    ;Ring buffer Full Counter byte Low
    RFCH    ;Ring buffer Full Counter byte High
	D60SC	;Device 1/60 Seconds Counter
	
	endc

INIT_DEVICE_COUNTERS
	CLRF LBCH	;LineBreak Counter byte High
	CLRF LBCL	;LineBreak Counter byte Low
	CLRF SBCL	;Sent Byte Counter byte Low
	CLRF SBCH	;Sent Byte Counter byte High
	CLRF SBCHH	;Sent Byte Counter byte High High
	CLRF RBCL	;Read Byte Counter byte Low
	CLRF RBCH	;Read Byte Counter byte High
	CLRF RBCHH	;Read Byte Counter byte High High
	CLRF RMCH	;Read Message Counter byte High
	CLRF RMCL	;Read Message Counter byte Low
	CLRF FECL	;Framing Error Counter byte Low
	CLRF FECH	;Framing Error Counter byte High
	CLRF CECL	;Checksum Error Counter byte Low
	CLRF CECH	;Checksum Error Counter byte High
	CLRF DSCL	;Device Seconds Counter byte Low
	CLRF DSCH	;Device Seconds Counter byte HIGH
	CLRF UAACH	;Unsuccessful Access Attempts Counter byte High
	CLRF UAACL	;Unsuccessful Access Attempts Counter byte Low
	CLRF SMCL	;Sent Message Counter byte Low
	CLRF SMCH	;Sent Message Counter byte High	
    CLRF OACL   ;Output Actions Counter byte Low
    CLRF OACH   ;Output Actions Counter byte High
    CLRF RFCL    ;Ring buffer Full Counter byte Low
    CLRF RFCH    ;Ring buffer Full Counter byte High
    MOVLW 60
	MOVWF D60SC	;Device 1/60 Seconds Counter
	RETURN
	

	
INCREMENT_LINEBREAK_COUNTER MACRO	;LineBreak Counter

		INCFSZ LBCL,F
		BRA $+4
		INCF LBCH,F
		NOP
	
		ENDM
	
INCREMENT_SENT_BYTE_COUNTER MACRO	;Sent Byte Counter

		INCFSZ SBCL,F
		BRA $+8
		INCFSZ SBCH,F
		BRA $+4
		INCF SBCHH,F
		NOP
		
		ENDM
		
INCREMENT_READ_BYTE_COUNTER MACRO	;Read Byte Counter

		INCFSZ RBCL,F
		BRA $+8
        INCFSZ RBCH,F
        BRA $+4
		INCF RBCHH,F
		NOP
	
		ENDM	

INCREMENT_READ_MESSAGE_COUNTER MACRO	;Read Message Counter

		INCFSZ RMCL,F
		BRA $+4
		INCF RMCH,F
		NOP
	
		ENDM

INCREMENT_FRAMING_ERROR_COUNTER MACRO	;Framing Error Counter

		INCFSZ FECL,F
		BRA $+4
		INCF FECH,F
		NOP
	
		ENDM

INCREMENT_CHECKSUM_ERROR_COUNTER MACRO	;Checksum Error Counter

		INCFSZ CECL,F
		BRA $+4
		INCF CECH,F
		NOP
	
		ENDM

INCREMENT_OUTPUT_ACTION_COUNTER MACRO	;Output Actions Counter

		INCFSZ OACL,F
		BRA $+4
		INCF OACH,F
		NOP
	
		ENDM

INCREMENT_Ring_Buffer_Full_COUNTER MACRO	;Ring Buffer Full Counter

		INCFSZ RFCL,F
		BRA $+4
		INCF RFCH,F
		NOP
	
		ENDM

INCREMENT_DEVICE_SECONDS_COUNTER MACRO	;Device Seconds Counter
		LOCAL DECFSZ_END

		DECFSZ D60SC,F
		BRA DECFSZ_END		
		MOVLW 60
		MOVWF D60SC
		INCFSZ DSCL,F
		BRA DECFSZ_END
		INCF DSCH,F
		
DECFSZ_END
		NOP
		ENDM

INCREMENT_UNSUCCESSFUL_ACCESS_ATTEMPTS_COUNTER MACRO ;Unsuccessful Access Attempts Counter

		INCFSZ UAACL,F
		BRA $+4
		INCF UAACH,F
		NOP
	
		ENDM

INCREMENT_SENT_MESSAGE_COUNTER MACRO	;Sent Message Counter

		INCFSZ SMCL,F
		BRA $+4
		INCF SMCH,F
		NOP
	
		ENDM	


;0xE5 BRON=CODE_DATA_DC1:  DATA:[RFCL,RFCH,SBCL,SBCH,SBCHH,RBCL,RBCH,RBCHH]
;0xF5 BRON=CODE_DATA_DC2:  DATA:[FECL,FECH,CECL,CECH,OACL,OACH,UAACL,UAACH]
;0xE5 BRON=CODE_DATA_DC3:  DATA:[SMCL,SMCH,RMCL,RMCH,DSCL,DSCH,LBCL,LBCH]
 
		
SENT_COUNTERS 

		CLRF TE_VERZENDEN_BYTE6
		CLRF TE_VERZENDEN_BYTE11

		MOVLW OPC_PEER_XFER				;     eerste te verzenden byte is opcode OPC_PEER_XFER  (E5)
		MOVWF TE_VERZENDEN_BYTE1

		MOVLW 0x10				;     tweede byte lengte datastroom =h10
		MOVWF TE_VERZENDEN_BYTE2

		MOVLW CODE_DATA_DC1
		MOVWF TE_VERZENDEN_BYTE3

		MOVF DEVICENR,W
		MOVWF TE_VERZENDEN_BYTE4

		MOVF POORTNR,W
		MOVWF TE_VERZENDEN_BYTE5	

		MOVF RFCL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,0
		MOVWF TE_VERZENDEN_BYTE7

		MOVF RFCH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,1
		MOVWF TE_VERZENDEN_BYTE8

		
		MOVF SBCL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,2
		MOVWF TE_VERZENDEN_BYTE9

		MOVF SBCH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,3
		MOVWF TE_VERZENDEN_BYTE10

		
		MOVF SBCHH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,0
		MOVWF TE_VERZENDEN_BYTE12

		MOVF RBCL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,1
		MOVWF TE_VERZENDEN_BYTE13

		
		MOVF RBCH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,2
		MOVWF TE_VERZENDEN_BYTE14

		MOVF RBCHH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,3
		MOVWF TE_VERZENDEN_BYTE15

        MOVLW 16
        movwf AANTAL_BYTES
		CALL TE_VERZENDEN_BYTES_IN_BUFFER	
				
;0xF5 BRON=CODE_DATA_DC2:  DATA:[FECL,FECH,CECL,CECH,OACL,OACH,UAACL,UAACH]
		
		CLRF TE_VERZENDEN_BYTE6
		CLRF TE_VERZENDEN_BYTE11

		MOVLW CODE_DATA_DC2
		MOVWF TE_VERZENDEN_BYTE3

		MOVF FECL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,0
		MOVWF TE_VERZENDEN_BYTE7

		MOVF FECH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,1
		MOVWF TE_VERZENDEN_BYTE8

		
		MOVF CECL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,2
		MOVWF TE_VERZENDEN_BYTE9

		MOVF CECH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,3
		MOVWF TE_VERZENDEN_BYTE10

		
		MOVF OACL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,0
		MOVWF TE_VERZENDEN_BYTE12

		MOVF OACH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,1
		MOVWF TE_VERZENDEN_BYTE13

		
		MOVF UAACL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,2
		MOVWF TE_VERZENDEN_BYTE14

		MOVF UAACH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,3
		MOVWF TE_VERZENDEN_BYTE15

        MOVLW 16
        movwf AANTAL_BYTES
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
		
;0xE5 BRON=CODE_DATA_DC3:  DATA:[SMCL,SMCH,RMCL,RMCH,DSCL,DSCH,LBCL,LBCH]		

		CLRF TE_VERZENDEN_BYTE6
		CLRF TE_VERZENDEN_BYTE11

		MOVLW CODE_DATA_DC3
		MOVWF TE_VERZENDEN_BYTE3

		MOVF SMCL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,0
		MOVWF TE_VERZENDEN_BYTE7

		MOVF SMCH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,1
		MOVWF TE_VERZENDEN_BYTE8

		
		MOVF RMCL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,2
		MOVWF TE_VERZENDEN_BYTE9

		MOVF RMCH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,3
		MOVWF TE_VERZENDEN_BYTE10

		
		MOVF DSCL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,0
		MOVWF TE_VERZENDEN_BYTE12

		MOVF DSCH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,1
		MOVWF TE_VERZENDEN_BYTE13

		
		MOVF LBCL,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,2
		MOVWF TE_VERZENDEN_BYTE14

		MOVF LBCH,W
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,3
		MOVWF TE_VERZENDEN_BYTE15

        MOVLW 16
        movwf AANTAL_BYTES
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
		
		RETURN