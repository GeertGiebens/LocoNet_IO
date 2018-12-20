
;***************************************************************************
;* LocoNet In/out module met PIC 18F4620 microcontroller                   *
;*                                                                         *
;* Geschreven door Geert Giebens voor PIC 18F4620 microcontroller          *
;*                                                                         *
;* Datum: 30 OKT 2018                      Versie LOCONET IO V1.1          *
;*                                                                         *
;* Filenaam: LOCONET_IO_RELAIS.INC                                         *
;*                                                                         *
;***************************************************************************

;RELAIS prodedure wordt 60 keer per seconde uitgevoerd.

;**********************************************************
;*******  MACRO RELAIS   **********************************
;**********************************************************


; FV1 = T1: als gewenste stand '1' wordt, dan is T1 wachttijd voordat uitgang aangestuurd wordt 1-255s 
;                                         als T1= 0s dan uitgang direct aansturen
; FV2 = T2: als T2 =0 dan uitgang volgt direct 'gewenste stand' na T1
;       T2: als FV3.bit2 = '0' uitgang zal T2 1-255s aangestuurd blijven en daarna uitschakelen, 
;                              uitgang zal direct uitschakelen als 'gewenste stand' laag wordt
;       T3: als FV3.bit2 = '1' uitgang zal T3 1-255s aangestuurd blijven nadat 'gewenste stand' laag is geworden
; FV3.bit0 = '0' dan uitgang actief = +5V
;          = '1' dan uitgang actief = MASSA
; FV3.bit1 = '0' dan uitgang eenmalig doorlopen
;          = '1' dan uitgang repeterend met T1 'UIT' en T2 'AAN' ... (enkel te gebruiken bij FV3.bit2='0' en T1 en T2 geen 0s)
; Var1 = telt van 60 tot 0 = 1s
; Var2 = secondenteller 1-255s tot 0s


;Waarden van BINAIR b'76543210'
;                      ||||| 
;                      ||||| 
;                      ||||b=0 T1 is voorbij
;                      |||b= vorige toestand bit4 BINAIR (detectie dat er een overgang is van '0' --> '1' -->'0'
;                      ||b=1 'Gewenste stand' uitgang
;                      |b=0 T2 is voorbij
;                      b=0 T3 is voorbij

MACRO_RELAIS MACRO RELAISNR,RELAISPOORT,RELAISPOORTBIT

    LOCAL RL_END, RL_SPR10, RL_SPR5, RL_SPR15, RL_SPR20, RL_SPR25, RL_SPR22, RL_SPR30, RL_SPR40


    BTFSS ((LOW TBL_FUNCTIE) + (RELAISNR-1)),3,BANKED	;Is functie relais?
    BRA RL_END                                          ;   Nee: dan volgende poort


    BTFSS ((LOW TBL_BINAIR) + (RELAISNR-1)),4,BANKED    ;Is gewenste stand spoel ='1'  (bit4)  
	BRA RL_SPR10                                        ; JA0:
   
	BTFSC ((LOW TBL_BINAIR) + (RELAISNR-1)),3,BANKED	;   JA: is er een overgang van bit4 (0-->1) 
	BRA RL_SPR5
	
    MOVLW b'01111100'                                   ;       JA: Herstart volledig timing proces met T1 
    MOVWF ((LOW TBL_BINAIR) + (RELAISNR-1)),BANKED	

	MOVF ((LOW TBL_CONST1) + (RELAISNR-1)),W,BANKED		;           is T1 (FV1)>'0' 
	BZ RL_SPR15                                         ;               JA1: dan start T1   NEE: start 'eventueel?' T2 -->SPR15
	MOVWF ((LOW TBL_VAR2) + (RELAISNR-1)),BANKED
	MOVLW 60
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	
    BTFSC ((LOW TBL_CONST3)+ (RELAISNR-1)),0,BANKED     ;                   RESET uitgang
    BRA $+6
    BCF RELAISPOORT,RELAISPOORTBIT                   
    BRA RL_END
    BSF RELAISPOORT,RELAISPOORTBIT	  
	BRA RL_END


RL_SPR5                                                 ;         NEE:
	BTFSS ((LOW TBL_BINAIR) + (RELAISNR-1)),2,BANKED    ;             Is T1 bezig?
	BRA RL_SPR25
	DECFSZ ((LOW TBL_VAR1) + (RELAISNR-1)),F,BANKED     ;                JA2: verminder T1 tot 0s dan start 'eventueel?' T2 -->SPR15 
	BRA RL_END
	MOVLW 60 
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	DECFSZ ((LOW TBL_VAR2) + (RELAISNR-1)),F,BANKED
	BRA RL_END
	

RL_SPR15
	BCF ((LOW TBL_BINAIR) + (RELAISNR-1)),2,BANKED      ;               NEE1: geef aan dat T1 voorbij is	
	
    BTFSC ((LOW TBL_CONST3)+ (RELAISNR-1)),0,BANKED     ;                    SET uitgang
    BRA $+6
    BSF RELAISPOORT,RELAISPOORTBIT                   
    BRA $+4 
    BCF RELAISPOORT,RELAISPOORTBIT

    BTFSC ((LOW TBL_CONST3)+ (RELAISNR-1)),2,BANKED     ;                    Als FV3.bit2=1 dan zal uitgang GESET blijven en einde T2 -->SPR22
	BRA RL_SPR22
	
	MOVF ((LOW TBL_CONST2) + (RELAISNR-1)),W,BANKED     ;                    laad waarde T2, Indien 0 dan zal uitgang GESET blijven en einde T2 -->SPR22
	BZ RL_SPR22 

	MOVWF ((LOW TBL_VAR2) + (RELAISNR-1)),BANKED        ;                    laad waarde voor T2
	MOVLW 60
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	BRA RL_END
	
RL_SPR22
    BCF ((LOW TBL_BINAIR) + (RELAISNR-1)),5,BANKED      ;                    geef aan dat T2 voorbij is		
	BRA RL_END
	
RL_SPR25
	BTFSS ((LOW TBL_BINAIR) + (RELAISNR-1)),5,BANKED    ;                NEE2:Is T2 bezig?
	BRA RL_END                                          ;                       NEE: einde
	DECFSZ ((LOW TBL_VAR1) + (RELAISNR-1)),F,BANKED		;                       JA: is T2 voorbij dan uitgang niet meer GESET		
	BRA RL_END
	MOVLW 60 
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	DECFSZ ((LOW TBL_VAR2) + (RELAISNR-1)),F,BANKED
	BRA RL_END

    BCF ((LOW TBL_BINAIR) + (RELAISNR-1)),5,BANKED			
    BTFSC ((LOW TBL_CONST3)+ (RELAISNR-1)),0,BANKED  
    BRA $+6
    BCF RELAISPOORT,RELAISPOORTBIT                  
    BRA $+4
    BSF RELAISPOORT,RELAISPOORTBIT 

    BTFSS ((LOW TBL_CONST3)+ (RELAISNR-1)),1,BANKED     ;                       Indien repeterend, dan herstart 
    BRA RL_END  
	MOVLW b'00010000'
    MOVWF ((LOW TBL_BINAIR) + (RELAISNR-1)),BANKED
    BRA RL_END    


RL_SPR10
	BTFSS ((LOW TBL_BINAIR) + (RELAISNR-1)),3,BANKED	;  NEE0: is er een overgang van bit4 (1-->0) 
	BRA RL_SPR30

    MOVLW b'01000000'                                   ;       JA: start timing proces T3
    MOVWF ((LOW TBL_BINAIR) + (RELAISNR-1)),BANKED	
	
    BTFSS ((LOW TBL_CONST3)+ (RELAISNR-1)),2,BANKED     ;            Als FV3.bit2=0 dan geen T3
    BRA RL_SPR40

	MOVF ((LOW TBL_CONST2) + (RELAISNR-1)),W,BANKED     ;      laad waarde T3, Indien 0 dan einde T3
	BZ RL_SPR40 

	MOVWF ((LOW TBL_VAR2) + (RELAISNR-1)),BANKED        ;           laad waarde voor T3
	MOVLW 60
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	BRA RL_END	

RL_SPR30
	BTFSS ((LOW TBL_BINAIR) + (RELAISNR-1)),6,BANKED    ;Is T3 bezig?
    BRA RL_END

	DECFSZ ((LOW TBL_VAR1) + (RELAISNR-1)),F,BANKED		;                     is T3 voorbij dan uitgang niet meer actief		
	BRA RL_END
	MOVLW 60 
	MOVWF ((LOW TBL_VAR1) + (RELAISNR-1)),BANKED
	DECFSZ ((LOW TBL_VAR2) + (RELAISNR-1)),F,BANKED
	BRA RL_END    
 
RL_SPR40
    BCF ((LOW TBL_BINAIR) + (RELAISNR-1)),6,BANKED      ; Einde T3 en RESET uitgangen
    BTFSC ((LOW TBL_CONST3)+ (RELAISNR-1)),0,BANKED
    BRA $+6
    BCF RELAISPOORT,RELAISPOORTBIT                  
    BRA RL_END
    BSF RELAISPOORT,RELAISPOORTBIT 


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
    IF DEBUGGER == 0
        MACRO_RELAIS 29,POORT29
        MACRO_RELAIS 30,POORT30
    ENDIF


        RETURN             