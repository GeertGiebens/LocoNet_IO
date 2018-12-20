

;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;* Datum: 29 Nov 2018                      Versie LOCONET IO V1.2          *
;*                                                                         *
;* Filenaam: LOCONET_IO_VARIABELEN_V1p2.INC                                *
;*                                                                         *
;***************************************************************************


;************  POORTEN   ********************************************

	#DEFINE POORT1 PORTA,RA1
	#DEFINE POORT2 PORTA,RA2
	#DEFINE POORT3 PORTA,RA5
	#DEFINE POORT4 PORTE,RE0
	#DEFINE POORT5 PORTE,RE1
	#DEFINE POORT6 PORTE,RE2

	#DEFINE POORT7  PORTA,RA7
	#DEFINE POORT8  PORTA,RA6
	#DEFINE POORT9  PORTC,RC0
	#DEFINE POORT10 PORTC,RC1
	#DEFINE POORT11 PORTC,RC2
	#DEFINE POORT12 PORTC,RC3
	#DEFINE POORT13 PORTD,RD0
	#DEFINE POORT14 PORTD,RD1

	#DEFINE POORT15 PORTD,RD2
	#DEFINE POORT16 PORTD,RD3
	#DEFINE POORT17 PORTC,RC4
	#DEFINE POORT18 PORTC,RC5
	#DEFINE POORT19 PORTD,RD4
	#DEFINE POORT20 PORTD,RD5
	#DEFINE POORT21 PORTD,RD6
	#DEFINE POORT22 PORTD,RD7

	#DEFINE POORT23 PORTB,RB0
	#DEFINE POORT24 PORTB,RB1
	#DEFINE POORT25 PORTB,RB2
	#DEFINE POORT26 PORTB,RB3
	#DEFINE POORT27 PORTB,RB4
	#DEFINE POORT28 PORTB,RB5
	#DEFINE POORT29 PORTB,RB6
	#DEFINE POORT30 PORTB,RB7

    #DEFINE PROGRAM_NEW_DEVICE_ADRES PORTE,3


;************  DEVICE FUNCTIES   ********************************************
	
	#DEFINE FUNCTIE_NOP         0x00
    #DEFINE FUNCTIE_SWITCH      0x01
    #DEFINE FUNCTIE_BUTTON      0x03
    #DEFINE FUNCTIE_BUTTON_ON   0x05
    #DEFINE FUNCTIE_BUTTON_OFF  0x07
    #DEFINE FUNCTIE_RELAY       0x08
    #DEFINE FUNCTIE_COIL1       0x10
    #DEFINE FUNCTIE_COIL2       0x12
    #DEFINE FUNCTIE_LED         0x40
    #DEFINE FUNCTIE_LED_BLK     0x42
    #DEFINE FUNCTIE_SERVO       0x80
    #DEFINE FUNCTIE_ON_OFF      0x21


;************  LOCONET OPDRACHTEN   *********************************

    #DEFINE OPC_SW_REQ      0xB0
    #DEFINE OPC_PEER_XFER   0xE5
    #DEFINE OPC_INPUT_REP   0xB2

    #DEFINE CODE_UPLOAD     0x10    ;master wil gegevens opvragen van DEVICENR,POORTNUMMER,ADRESNR
    #DEFINE CODE_UPDATE     0x11    ;master wil gegevens updaten van DEVICENR,POORTNUMMER,ADRESNR
    #DEFINE CODE_DATA       0x12    ;DEVICE stuurt de gevraagde gegevens door
    #DEFINE CODE_RESET      0x13    ;Reset Device
    #DEFINE CODE_COUNTERS   0x14    ;Master vraagt DEVICE COUNTERS op
    #DEFINE CODE_UPDATE_L   0x15    ;Master wil gegevens updaten Logische poorten
    #DEFINE CODE_UPLOAD_L	0x16    ;Master wil logische gegevens poorten opvragen 
    #DEFINE CODE_DATA_L     0x17    ;DEVICE stuurt de gevraagde gegevens Logische poort door
    #DEFINE CODE_DATA_DC1   0x20    ;DEVICE COUNTERS:[LBCL,LBCH,SBCL,SBCH,SBCHH,RBCL,RBCH,RBCHH]
    #DEFINE CODE_DATA_DC2   0x21    ;DEVICE COUNTERS:[FECL,FECH,CECL,CECH,DSCL,DSCH,UAACL,UAACH]
    #DEFINE CODE_DATA_DC3   0x22    ;DEVICE COUNTERS:[SMCL,SMCH,RMCL,RMCH,OACL,OACH,RFCL,RFCH]
    #DEFINE CODE_RESET_C    0x30    ;RESET Device Status Counters


;************ VARIABELEN ********************************************

;RAM geheugen indeling:

; BANK0  00-h6F  algemene variabelen
; BANK0  h70-h7F LocoNet variabelen
; BANK1: 0-255   TE VERZENDEN DATA LocoNet(RING BUFFER 256 BYTES)
; BANK2  00-31   TBL_FUNCTIE
; BANK2  32-63   TBL_CONST1
; BANK2  64-95	 TBL_CONST2
; BANK2  96-127  TBL_CONST3
; BANK2  128-159 TBL_BINAIR
; BANK2  160-191 TBL_VAR1
; BANK2  192-223 TBL_VAR2
; BANK2  224-255 TBL_INSTELLINGEN
; BANK3  128-255 RECEIVE BUFFER LocoNet(128 BYTES)
; BANK3  000-127 TRANSMIT BUFFER LocoNet(128 BYTES)
; BANK4  00-31   TBL_ADR1L	;LocoNet adres 1 Poort 1-32 Low byte
; BANK4  32-63   TBL_ADR1H	;LocoNet adres 1 Poort 1-32 High byte
; BANK4  64-95   TBL_ADR2L	;LocoNet adres 2 Poort 1-32 Low byte
; BANK4  96-127  TBL_ADR2H	;LocoNet adres 2 Poort 1-32 High byte
; BANK4  ...
; BANK6  192-223 TBL_ADR12L	;LocoNet adres 12 Poort 1-32 Low byte
; BANK6  224-255 TBL_ADR12H	;LocoNet adres 12 Poort 1-32 High byte 


	cblock 0x00	;algemene variabelen

POORTNUMMER 		;Poortnummer= 1-30
DEVICENUMMER		;Device nummer 1-127 opgeslagen in EEPROM

SERVO_TEL	    	;houd bij hoeveel servo's in beweging zijn startwaarde =1 tot MAXIMUM_SERVO 

HULPTEL		    	;teller algemeen gebruik
KNIPPERTEL          ;houd bij of LED moet knipperen
LED_FLAG            ;algemene flag voor gebruik bij LED
TABEL_GOTO          ;gebruikt bij LED om GOTO te berekenen
COPY_C              ;gebruikt bij LED om fade back juist teberekenen

TE_VERZENDEN_BYTE1  ;gebruikt om data op te slaan in te verzenden data buffer
TE_VERZENDEN_BYTE2
TE_VERZENDEN_BYTE3
TE_VERZENDEN_BYTE4
TE_VERZENDEN_BYTE5
TE_VERZENDEN_BYTE6
TE_VERZENDEN_BYTE7
TE_VERZENDEN_BYTE8
TE_VERZENDEN_BYTE9
TE_VERZENDEN_BYTE10
TE_VERZENDEN_BYTE11
TE_VERZENDEN_BYTE12
TE_VERZENDEN_BYTE13
TE_VERZENDEN_BYTE14
TE_VERZENDEN_BYTE15
TE_VERZENDEN_EXORBYTE
AANTAL_BYTES        ;aantal bytes te verzenden (is eerste byte in tabel daarna te verzenden bytes
POSITIE_TELLER_IN   ;waar staat pointer bij data in 0-->255    
POSITIE_TELLER_OUT   ;waar staat pointer bij data out
VERSCHIL_TELLER     ;verschil tussen POSITIE_TELLER_OUT en POSITIE_TELLER_IN mag niet meer worden dan 255


;Variabelen gebruikt bij decodeer Loconet opdracht:

LN_BYTE_RECEIVE1
LN_BYTE_RECEIVE2
LN_BYTE_RECEIVE3
LN_BYTE_RECEIVE4
LN_BYTE_RECEIVE5
LN_BYTE_RECEIVE6
LN_BYTE_RECEIVE7
LN_BYTE_RECEIVE8
LN_BYTE_RECEIVE9
LN_BYTE_RECEIVE10
LN_BYTE_RECEIVE11
LN_BYTE_RECEIVE12
LN_BYTE_RECEIVE13
LN_BYTE_RECEIVE14
LN_BYTE_RECEIVE15

BRONCODE
DEVICENR
POORTNR
ADRESNR
FUNCTIE
CONST1
CONST2
CONST3
BINAIR
ADRESLOW
ADRESHIGH

POORT_TELLER		;houd bij welk poort en adres er verwerkt wordt
POORT_FUNCTIE		;copy functie poort

;cblock 0x70-0x7F -->  LocoNet variabele zie LOCONET_18F4X20_V1p2.INC

	endc

;********************************************************************
;************ TABELLEN **********************************************
;********************************************************************

    CBLOCK 0x100
TE_VERZENDEN_DATA       ;256 BYTES
    ENDC

	CBLOCK 0x200
TBL_FUNCTIE   ;0x21F = Device Adres!
	endc

	CBLOCK 0x220
TBL_CONST1
	endc

	CBLOCK 0x240
TBL_CONST2
	endc

	CBLOCK 0x260
TBL_CONST3
	endc

	CBLOCK 0x280
TBL_BINAIR
	endc

	CBLOCK 0x2A0
TBL_VAR1
	endc

	CBLOCK 0x2C0
TBL_VAR2
	endc

	CBLOCK 0x2E0
TBL_Logische_Functies
	endc  

;	cblock 0x300
;LN_TRANSMIT_BUFFER			; LocoNet Transmit buffer zie LOCONET_18F4X20_V1p2.INC
;	endc

;	cblock 0x380
;LN_RECEIVE_BUFFER			; LocoNet Receive buffer zie LOCONET_18F4X20_V1p2.INC
;	endc

	CBLOCK 0x400
TBL_ADR1L
	ENDC

	CBLOCK 0x420
TBL_ADR1H
	ENDC

	CBLOCK 0x440
TBL_ADR2L
	ENDC

	CBLOCK 0x460
TBL_ADR2H
	ENDC

	CBLOCK 0x480
TBL_ADR3L
	ENDC

	CBLOCK 0x4A0
TBL_ADR3H
	ENDC

	CBLOCK 0x4C0
TBL_ADR4L
	ENDC

	CBLOCK 0x4E0
TBL_ADR4H
	ENDC

	CBLOCK 0x500
TBL_ADR5L
	ENDC

	CBLOCK 0x520
TBL_ADR5H
	ENDC

	CBLOCK 0x540
TBL_ADR6L
	ENDC

	CBLOCK 0x560
TBL_ADR6H
	ENDC

	CBLOCK 0x580
TBL_ADR7L
	ENDC

	CBLOCK 0x5A0
TBL_ADR7H
	ENDC

	CBLOCK 0x5C0
TBL_ADR8L
	ENDC

	CBLOCK 0x5E0
TBL_ADR8H
	ENDC

	CBLOCK 0x600
TBL_ADR9L
	ENDC

	CBLOCK 0x620
TBL_ADR9H
	ENDC

	CBLOCK 0x640
TBL_ADR10L
	ENDC

	CBLOCK 0x660
TBL_ADR10H
	ENDC

	CBLOCK 0x680
TBL_ADR11L
	ENDC

	CBLOCK 0x6A0
TBL_ADR11H
	ENDC

	CBLOCK 0x6C0
TBL_ADR12L
	ENDC

	CBLOCK 0x6E0
TBL_ADR12H
	ENDC