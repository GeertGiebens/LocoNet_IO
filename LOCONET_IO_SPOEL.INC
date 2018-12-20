
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
;* Datum: 27 Mei 2018                     Versie LOCONET IO V1.2           *
;*                                                                         *
;* Filenaam: LOCONET_IO_SPOEL.INC                                          *
;*                                                                         *
;***************************************************************************


;**********************************************************
;*******  MACRO SPOEL   ***********************************
;**********************************************************

;Waarden van BINAIR b'76543210'
;                        |  ||
;                        |  |b=1 Spoel actief
;                        |  b=1 Huidige stand spoel
;                        b=1 Gewenste stand spoel


MACRO_SPOEL MACRO SPOELNR,SPOELPOORT,SPOELPOORTBIT

        LOCAL MS_END, MS_SPR10,MS_SPR20,MS_SPR15, MS_SPR13, MS_SPR12, MS_SPR5, MS_SPR6

		BTFSS ((LOW TBL_FUNCTIE) + (SPOELNR-1)),4,BANKED	;Is functie spoel?
		BRA MS_END                                          ;   Nee: dan volgende poort

        BTFSS ((LOW TBL_BINAIR) + (SPOELNR-1)),0,BANKED     ;Is spoel actief dan uitgang =0 en stop timer0
        BRA MS_SPR10

        BTFSC ((LOW TBL_CONST3)+ (SPOELNR-1)),0,BANKED
        BRA MS_SPR5
        BCF SPOELPOORT,SPOELPOORTBIT                 
        BRA MS_SPR6
MS_SPR5
        BSF SPOELPOORT,SPOELPOORTBIT
MS_SPR6 
        BSF ((LOW TBL_BINAIR) + (SPOELNR-1)),1,BANKED
        BCF T0CON,TMR0ON  
        BCF INTCON,TMR0IF  
        BCF ((LOW TBL_BINAIR) + (SPOELNR-1)),0,BANKED 
        BRA MS_END

MS_SPR10
        BTFSC T0CON,TMR0ON                                  ;is timer aangestuurd door andere spoel dan doe niets
        BRA MS_END

        BTFSS ((LOW TBL_BINAIR) + (SPOELNR-1)),4,BANKED     ;Is gewenste stand spoel =0 dan doe niets
        BRA MS_SPR20

        BTFSC ((LOW TBL_BINAIR) + (SPOELNR-1)),1,BANKED     ;Staat de spoel al in de gevraagde stand, dan doe niets
        BRA MS_END
        BTFSC ((LOW TBL_CONST3)+ (SPOELNR-1)),0,BANKED
        BRA MS_SPR12
        BSF SPOELPOORT,SPOELPOORTBIT                 
        BRA MS_SPR13
MS_SPR12
        BCF SPOELPOORT,SPOELPOORTBIT
MS_SPR13 
                    
        MOVF ((LOW TBL_CONST1) + (SPOELNR-1)),W,BANKED
        COMF WREG,W  
        MOVWF TMR0H  
        SETF TMR0L  
        BSF T0CON,TMR0ON  
        BSF ((LOW TBL_BINAIR) + (SPOELNR-1)),0,BANKED 
        BTFSS ((LOW TBL_FUNCTIE)+(SPOELNR-1)),1,BANKED         ;Reset gewenste stand andere spoel
        BRA MS_SPR15
        BCF ((LOW TBL_BINAIR) + (SPOELNR-2)),4,BANKED
        RETURN
MS_SPR15
        BCF ((LOW TBL_BINAIR) + (SPOELNR)),4,BANKED
        RETURN                                               ;   en einde SPOEL program
MS_SPR20
        BCF ((LOW TBL_BINAIR) + (SPOELNR-1)),1,BANKED
MS_END
        NOP
        ENDM

;**********************************************************

SPOEL
        BTFSS T0CON,TMR0ON        ;Is Timer0 ON en Geen INT Timer0 dan doe niets
        BRA SP_SPR10
        BTFSS INTCON,TMR0IF  
        RETURN

SP_SPR10
        MOVLB HIGH TBL_FUNCTIE  ;selecteer bank waar de tabellen staan

        MACRO_SPOEL 1,POORT1
        MACRO_SPOEL 2,POORT2
        MACRO_SPOEL 3,POORT3
        MACRO_SPOEL 4,POORT4
        MACRO_SPOEL 5,POORT5
        MACRO_SPOEL 6,POORT6
        MACRO_SPOEL 7,POORT7
        MACRO_SPOEL 8,POORT8
        MACRO_SPOEL 9,POORT9
        MACRO_SPOEL 10,POORT10
        MACRO_SPOEL 11,POORT11
        MACRO_SPOEL 12,POORT12
        MACRO_SPOEL 13,POORT13
        MACRO_SPOEL 14,POORT14
        MACRO_SPOEL 15,POORT15
        MACRO_SPOEL 16,POORT16
        MACRO_SPOEL 18,POORT18
        MACRO_SPOEL 19,POORT19
        MACRO_SPOEL 20,POORT20
        MACRO_SPOEL 21,POORT21
        MACRO_SPOEL 22,POORT22
        MACRO_SPOEL 23,POORT23
        MACRO_SPOEL 24,POORT24
        MACRO_SPOEL 25,POORT25
        MACRO_SPOEL 26,POORT26
        MACRO_SPOEL 27,POORT27
        MACRO_SPOEL 28,POORT28
    IF DEBUGGER == 0
        MACRO_SPOEL 29,POORT29
        MACRO_SPOEL 30,POORT30
    ENDIF

        RETURN