
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
;* Datum: 6 JAN 2017                      Versie LOCONET IO V1.1           *
;*                                                                         *
;* Filenaam: LOCONET_IO_PROCEDURES.INC                                     *
;*                                                                         *
;***************************************************************************


;********************************************************************************
;*******  LAAD DATA IN EEPROM  **************************************************
;********************************************************************************

;EEADRH, EEADRL en EE_DATA moeten al op voorhand ingevuld worden

COPY_NAAR_EEPROM
        BCF EECON1,EEPGD
        BCF EECON1,CFGS
        BSF EECON1,WREN
        BCF INTCON,GIE
        MOVLW 55h
        MOVWF EECON2
        MOVLW 0AAh
        MOVWF EECON2
        BSF EECON1,WR
        BSF INTCON,GIE
        BTFSC EECON1,WR
        BRA $-2
        BCF EECON1,WREN
        RETURN




;***************************************************
;*******  Plaats data in te verzenden buffer   *****
;***************************************************


;TE_VERZENDEN_BYTE1  ;gebruikt om data op te slaan in te verzenden data buffer
;...
;TE_VERZENDEN_BYTE15
;TE_VERZENDEN_EXORBYTE
;AANTAL_BYTES        ;aantal bytes te verzenden (is eerste byte in tabel daarna te verzenden bytes
;POSITIE_TELLER_IN   ;waar staat pointer bij data in 0-->255    
;POSITIE_TELLER_OUT   ;waar staat pointer bij data out
;VERSCHIL_TELLER

;    CBLOCK 0xD00= TE_VERZENDEN_DATA



TE_VERZENDEN_BYTES_IN_BUFFER


    MOVF VERSCHIL_TELLER,W              ;als er een kans bestaat dat er meer dan 255 BYTES nog in te verzenden buffer staan, dan data niet in buffer
    ADDWF AANTAL_BYTES,W
    BNC SPR001
    RETURN  
SPR001
    CLRF TE_VERZENDEN_EXORBYTE
    LFSR 0,TE_VERZENDEN_DATA
    LFSR 1,TE_VERZENDEN_BYTE1
    MOVF POSITIE_TELLER_IN,W
    MOVWF FSR0L
    INCF POSITIE_TELLER_IN,F
    MOVF AANTAL_BYTES,W
    MOVWF INDF0
    MOVWF HULPTEL
    INCF VERSCHIL_TELLER,F
    DECF HULPTEL,F  

LOOP_TVBIB
    INCF FSR0L,F
    INCF POSITIE_TELLER_IN,F
    INCF VERSCHIL_TELLER,F  
    MOVF INDF1,W
    MOVWF INDF0
    XORWF TE_VERZENDEN_EXORBYTE,F   
    INCF FSR1L,F
    DECFSZ HULPTEL,F
    BRA LOOP_TVBIB

    INCF FSR0L,F
    INCF POSITIE_TELLER_IN,F
    INCF VERSCHIL_TELLER,F  
    MOVF TE_VERZENDEN_EXORBYTE,W
    COMF WREG,W
    MOVWF INDF0    

    RETURN


