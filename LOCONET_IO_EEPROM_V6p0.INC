

;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 27 JUN 2019                           Versie LOCONET IO V6.0    *
;*                                                                         *
;* Filenaam: LOCONET_IO_EEPROM_V6p0.INC                                    *
;*                                                                         *
;***************************************************************************

;Initiatie EEPROM met Data voor eerste gebruik �C

;CODE FUNCTION:
;00000000  	0x00	NOP
;00000001	0x01	SWITCH
;00000011	0x03	BUTTON
;00000101	0x05	BUTTON1
;00000111	0x07	BUTTON2
;00100001	0x21	BUTTON ON/OFF
;00001000	0x08	DIGITAL OUT
;00010000	0x10	COIL1
;00010010	0x12	COIL2
;01000000	0x40	PWM/LED
;01000010	0x42	LED BLINKING
;10000000	0x80	SERVO

        #DEFINE EEPROM_ADR_LOW_FUNCTION     0x00
        #DEFINE EEPROM_ADR_LOW_CONST1       0x20
        #DEFINE EEPROM_ADR_LOW_CONST2       0x40
        #DEFINE EEPROM_ADR_LOW_CONST3       0x60
        #DEFINE EEPROM_ADR_LOW_BINAIR       0x80
        #DEFINE EEPROM_ADR_LOW_ADR1L        0xA0
        #DEFINE EEPROM_ADR_LOW_ADR1H        0xC0

;EEPROM kan op voorhand al ingesteld worden met diverse mogelijkheden voor autonome PCB:  1 MOGELIJKHEID KAN ER MAAR '1' ZIJN!

        #DEFINE EEPROM_NOP                 0
        #DEFINE SERVO_RELAIS_SWITCH        0    ; POORT 7-13=SERVO, POORT 17-22=RELAIS, POORT 23-30=SWITCH
        #DEFINE SERVO_RELAIS_TOGGLE_SWITCH 0    ; POORT 7-13=SERVO, POORT 17-22=RELAIS, POORT 23-30=TOGGLE_SWITCH
        #DEFINE RELAIS_ADR_1_TOT_30        0    ; LOCONET uitgang RELAIS reageert op OPC_SW_REQ adres 1 tot 30
        #DEFINE LED_ADR_1_TOT_30           1    ; LOCONET uitgang LED reageert op OPC_SW_REQ adres 1 tot 30
        #DEFINE S88_1_TOT_30               0    ; S88 ingang met OPC_INPUT_REP
        #DEFINE DRUKKNOP_7_LED_14_SPOEL    0    ; 7 drukknoppen die 7 LED uitgangen aansturen en 14 relaisspoelen


    IF EEPROM_NOP == 1

	ORG	0xf00000

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FUNCTION
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 ; deze laatse is Device nummer

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV1
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV2
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV3
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;BINAIR    
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1H
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

    ENDIF

    IF SERVO_RELAIS_SWITCH == 1

	ORG	0xf00000

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80 ;FUNCTION
        DE  0x80,0x80,0x80,0x80,0x80,0x80,0x08,0x08
        DE  0x08,0x08,0x08,0x08,0x08,0x08,0x01,0x01
        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01 ; deze laatse is Device nummer

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0065,0065 ;FV1
        DE  0065,0065,0065,0065,0065,0065,0x02,0x02
        DE  0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0250,0250 ;FV2
        DE  0250,0250,0254,0250,0250,0250,0x02,0x02
        DE  0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF ;FV3
        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x04,0x04
        DE  0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;BINAIR    
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
        DE  0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x01
        DE  0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1H
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

    ENDIF

    IF SERVO_RELAIS_TOGGLE_SWITCH == 1

	ORG	0xf00000

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80 ;FUNCTION
        DE  0x80,0x80,0x80,0x80,0x80,0x80,0x08,0x08
        DE  0x08,0x08,0x08,0x08,0x08,0x08,0x03,0x03
        DE  0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x01 ; deze laatse is Device nummer

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0100,0100 ;FV1
        DE  0100,0100,0100,0100,0100,0100,0x01,0x01
        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0200,0200 ;FV2
        DE  0200,0200,0200,0200,0200,0200,0x01,0x01
        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF ;FV3
        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x04,0x04
        DE  0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;BINAIR    
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 ;ADR1L
        DE  0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x01
        DE  0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x01
        DE  0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1H
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

    ENDIF

    IF RELAIS_ADR_1_TOT_30 == 1

	ORG	0xf00000

        DE  0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08 ;FUNCTION
        DE  0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08
        DE  0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08
        DE  0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x01 ; deze laatse is Device nummer

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV1
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV2
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV3
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;BINAIR    
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0000,0001,0002,0003,0004,0005,0006,0007 ;ADR1L
        DE  0008,0009,0010,0011,0012,0013,0014,0015 
        DE  0016,0017,0018,0019,0020,0021,0022,0023 
        DE  0024,0025,0026,0027,0028,0029,0000,0000 

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1H
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

    ENDIF

    IF LED_ADR_1_TOT_30 == 1

	ORG	0xf00000

        DE  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40 ;FUNCTION
        DE  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40
        DE  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40
        DE  0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x01 ; deze laatse is Device nummer

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV1
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF ;FV2
        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00

        DE  0105,0105,0105,0105,0105,0105,0105,0105 ;FV3
        DE  0105,0105,0105,0105,0105,0105,0105,0105
        DE  0105,0105,0105,0105,0105,0105,0105,0105
        DE  0105,0105,0105,0105,0105,0105,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;BINAIR    
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0000,0001,0002,0003,0004,0005,0006,0007 ;ADR1L
        DE  0008,0009,0010,0011,0012,0013,0014,0015 
        DE  0016,0017,0018,0019,0020,0021,0022,0023 
        DE  0024,0025,0026,0027,0028,0029,0000,0000 

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1H
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

    ENDIF

    IF S88_1_TOT_30  == 1

	ORG	0xf00000

        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 ;FUNCTION
        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01
        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01
        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01 ; deze laatse is Device nummer

        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 ;FV1
        DE  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01
        DE  0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02
        DE  0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00

        DE  0001,0002,0003,0004,0005,0006,0007,0008, ;FV2
        DE  0009,0010,0011,0012,0013,0014,0015,0016
        DE  0001,0002,0003,0004,0005,0006,0007,0008, 
        DE  0009,0010,0011,0012,0013,0014,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV3
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;BINAIR    
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F ;ADR1H
        DE  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F
        DE  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F
        DE  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

    ENDIF

    IF DRUKKNOP_7_LED_14_SPOEL == 1

	ORG	0xf00000

        DE  0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x40 ;FUNCTION
        DE  0x40,0x40,0x40,0x40,0x40,0x40,0x10,0x12
        DE  0x10,0x12,0x10,0x12,0x10,0x12,0x10,0x12
        DE  0x10,0x12,0x10,0x12,0x00,0x00,0x00,0x01 ; deze laatse is Device nummer

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;FV1
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF
        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
        DE  0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF ;FV2
        DE  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0107 ;FV3   
        DE  0107,0107,0107,0107,0107,0107,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;BINAIR
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x00 ;ADR1L
        DE  0x01,0x02,0x03,0x04,0x05,0x06,0x00,0x00
        DE  0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04
        DE  0x05,0x05,0x06,0x06,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR1H
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

    ENDIF


        ORG 0xf00100
		
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;Niet te gebruiken (wordt overschreven in RAM door ADR1L)   
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;Niet te gebruiken (wordt overschreven in RAM door ADR1H)       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR2L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR2H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR3L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR3H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR4L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR4H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        ORG 0xf00200
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR5L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR5H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR6L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR6H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR7L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR7H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR8L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR8H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        ORG 0xf00300
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR9L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR9H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR10L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR10H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR11L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR11H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR12L
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ;ADR12H       
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        DE  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
		