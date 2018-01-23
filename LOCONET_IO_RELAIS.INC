
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
;* Datum: 23 JUL 2017                      Versie LOCONET IO V1.1          *
;*                                                                         *
;* Filenaam: LOCONET_IO_RELAIS.INC                                         *
;*                                                                         *
;***************************************************************************


;**********************************************************
;*******  MACRO RELAIS   **********************************
;**********************************************************

; Const1 = T1 wachttijd tijd voordat relais aangestuurd wordt 0-255s 0s is direct
; Const2 = T2 tijd dat relais aangestuurd wordt 1-255s  waarbij 0 altijd is totdat LN opdracht geeft om niet meer aan te sturen
; Var1 = telt van 60 tot 0 = 1s
; Var2 = secondenteller 0-255s 

;RELAIs prodedure wordt 60 keer per seconde uitgevoerd

;Waarden van BINAIR b'76543210'
;                       |||| b=1 waarde T2 is 0 (relais blijft actief)
;                       |||| 
;                       |||b=1 T1 is bezig
;                       ||b= vorige toestand bit4 BINAIR (detectie dat er een overgang is van '0' --> '1' 
;                       |b=1 Gewenste stand spoel
;                       b=1 T2 is bezig

MACRO_RELAIS MACRO RELAISNR,RELAISPOORT,RELAISPOORTBIT

        LOCAL RL_END, RL_SPR10, RL_SPR5, RL_SPR15, RL_SPR20, RL_SPR25

        BTFSS ((LOW TBL_FUNCTIE) + (RELAISNR-1)),3,BANKED	;Is functie relais?
        BRA RL_END                                          ;   Nee: dan volgende poort

        BTFSS ((LOW TBL_BINAIR) + (RELAISNR-1)),4,BANKED     
	BRA RL_SPR10

	BTFSC ((LOW TBL_BINAIR) + (RELAISNR-1)),3,BANKED	; is er een nieuwe overgang van bit4 0-->1 dan start eventueel timing proces
	BRA RL_SPR5
	BSF ((LOW TBL_BINAIR) + (RELAISNR-1)),3,BANKED
	BSF ((LOW TBL_BINAIR) + (RELAISNR-1)),2,BANKED

	MOVF ((LOW TBL_CONST1) + (RELAISNR-1)),W,BANKED		; is T1 (CONST1) =0 dan volgende stap anders start T1
	BZ RL_SPR15
	MOVWF ((LOW TBL_VAR2) + (RELAISNR-1)),BANKED
	MOVLW 60
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	BRA RL_END


RL_SPR5
	BTFSC ((LOW TBL_BINAIR) + (RELAISNR-1)),0,BANKED	;is uitgang actief, en moet deze actief blijven -->doe niks
	BRA RL_END

	BTFSC ((LOW TBL_BINAIR) + (RELAISNR-1)),5,BANKED	;Is T2 bezig?
	BRA RL_SPR25						; NEE: verminder T1
	DECFSZ ((LOW TBL_VAR1) + (RELAISNR-1)),F,BANKED
	BRA RL_END
	MOVLW 60 
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	DECFSZ ((LOW TBL_VAR2) + (RELAISNR-1)),F,BANKED
	BRA RL_END
	

RL_SPR15
	BSF ((LOW TBL_BINAIR) + (RELAISNR-1)),5,BANKED		; is T1 voorbij dan start T2  is T2 (const2) = 0 dan blijft uitgang actief
        BSF RELAISPOORT,RELAISPOORTBIT
	MOVF ((LOW TBL_CONST2) + (RELAISNR-1)),W,BANKED
	BNZ RL_SPR20
	
	BSF ((LOW TBL_BINAIR) + (RELAISNR-1)),0,BANKED
	BRA RL_END	

RL_SPR20
	MOVWF ((LOW TBL_VAR2) + (RELAISNR-1)),BANKED
	MOVLW 60
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	BRA RL_END

RL_SPR25
	DECFSZ ((LOW TBL_VAR1) + (RELAISNR-1)),F,BANKED		; is T2 voorbij dan uitgang niet meer actief		
	BRA RL_END
	MOVLW 60 
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	DECFSZ ((LOW TBL_VAR2) + (RELAISNR-1)),F,BANKED
	BRA RL_END

RL_SPR10
	BCF RELAISPOORT,RELAISPOORTBIT
	CLRF ((LOW TBL_BINAIR) + (RELAISNR-1)),BANKED	
	

RL_END
        NOP
        ENDM

;**********************************************************

RELAIS

        MOVLB HIGH TBL_FUNCTIE  ;selecteer bank waar de tabellen staan

        MACRO_RELAIS 1,POORT1
        MACRO_RELAIS 2,POORT2
        MACRO_RELAIS 3,POORT3
        MACRO_RELAIS 4,POORT4
        MACRO_RELAIS 5,POORT5
        MACRO_RELAIS 6,POORT6
        MACRO_RELAIS 7,POORT7
        MACRO_RELAIS 8,POORT8
        MACRO_RELAIS 9,POORT9
        MACRO_RELAIS 10,POORT10
        MACRO_RELAIS 11,POORT11
        MACRO_RELAIS 12,POORT12
        MACRO_RELAIS 13,POORT13
        MACRO_RELAIS 14,POORT14
        MACRO_RELAIS 15,POORT15
        MACRO_RELAIS 16,POORT16
        MACRO_RELAIS 17,POORT17
        MACRO_RELAIS 18,POORT18
        MACRO_RELAIS 19,POORT19
        MACRO_RELAIS 20,POORT20
        MACRO_RELAIS 21,POORT21
        MACRO_RELAIS 22,POORT22
        MACRO_RELAIS 23,POORT23
        MACRO_RELAIS 24,POORT24
        MACRO_RELAIS 25,POORT25
        MACRO_RELAIS 26,POORT26
        MACRO_RELAIS 27,POORT27
        MACRO_RELAIS 28,POORT28
    IF DEBUG == 0
        MACRO_RELAIS 29,POORT29
        MACRO_RELAIS 30,POORT30
    ENDIF


        RETURN             