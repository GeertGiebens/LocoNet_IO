
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
;* Datum: 03 APR 2017                      Versie LOCONET IO V1.1          *
;*                                                                         *
;* Filenaam: LOCONET_IO_PROGRAMMEER.INC                                    *
;*                                                                         *
;***************************************************************************


;***************************************************************************

CODEER_NAAR_TE_VERZENDEN_BYTES

        CLRF TE_VERZENDEN_BYTE6
        CLRF TE_VERZENDEN_BYTE11

		MOVLW 0xE5			    	;eerste te verzenden byte is opcode OPC_PEER_XFER  (E5)
		MOVWF TE_VERZENDEN_BYTE1

		MOVLW 0x10			    	;tweede byte lengte datastroom =h10
		MOVWF TE_VERZENDEN_BYTE2

		MOVF BRON,W			    	;derde byte BRON
		MOVWF TE_VERZENDEN_BYTE3

		MOVF DEVICENR,W				;vierde byte DEVICE NR
		MOVWF TE_VERZENDEN_BYTE4

		MOVF POORTNR,W				;vijfde byte POORT NR 
		MOVWF TE_VERZENDEN_BYTE5

		MOVF ADRESNR,W              ;zevende byte ADRES NR  
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,0
		MOVWF TE_VERZENDEN_BYTE7

		MOVF FUNCTIE,W              ;achste byte FUNCTIE
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,1
		MOVWF TE_VERZENDEN_BYTE8

		MOVF CONST1,W              ;negenste byte CONST1  
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,2
		MOVWF TE_VERZENDEN_BYTE9

		MOVF CONST2,W              ;tiende byte CONST2  
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE6,3
		MOVWF TE_VERZENDEN_BYTE10

		MOVF CONST3,W              ;twaalfde byte CONST3  
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,0
		MOVWF TE_VERZENDEN_BYTE12

		MOVF BINAIR,W              ;dertiende byte BINAIR  
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,1
		MOVWF TE_VERZENDEN_BYTE13

		MOVF ADRESLOW,W              ;vertiende byte ADRESLOW  
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,2
		MOVWF TE_VERZENDEN_BYTE14

		MOVF ADRESHIGH,W              ;vijftiende byte ADRESHIGH 
		BTFSS WREG,7
		BRA $+6
		BCF WREG,7
		BSF TE_VERZENDEN_BYTE11,3
		MOVWF TE_VERZENDEN_BYTE15

        MOVLW 16
        movwf AANTAL_BYTES
		CALL TE_VERZENDEN_BYTES_IN_BUFFER
		RETURN

		
;**********************************************************************

INIT_PROGRAMMEER;

	CLRF FLAG_PROG



	RETURN

	
;**********************************************************************

	#DEFINE STANDBY			FLAG_PROG,0     ;=1 Enkel de toetsen van STANDBY kunnen ingedrukt worden

	#DEFINE NUM_INVOER		FLAG_PROG,1     ;=1 als er data wordt ingevoerd numeriek
 
	#DEFINE PIJL_INVOER     FLAG_PROG,2     ;=1 als data wordt ingevoerd met pijltjestoetsen

											;Na INIT is FLAG_PROG = '00000000'

;**********************************************************************

;Enkel als er een nieuwe toets is ingedrukt, wordt dit programma uitgevoerd

; Toetsenvolgorde:
;
;    1  2  3  4        F1 F2  #  *
;    5  6  7  8         1  2  3  /\
;    9  10 11 12        4  5  6  \/   
;    13 14 15 16        7  8  9 Esc
;    17 18 19 20        <  0  > Ent


PROGRAMMEER

	BTFSS FLAG_KEY,0		        ;Indien er een nieuwe toets is ingedrukt doe:
	return

	BCF FLAG_KEY,0

	BTFSC STANDBY		        	;indien in STANDBY, ga naar STANDBY
	GOTO STANDBY_SPR

	BTFSC PIJL_INVOER		        ;indien in PIJL INVOER, ga naar PIJL INVOER
	GOTO PIJL_INVOER_SPR

	BTFSC NUM_INVOER		        ;indien in NUM INVOER, ga naar NUM INVOER
	GOTO NUM_INVOER_SPR

					                 ;Anders: Vraag gegevens #XX van *01 van adres 01 en zet in  STANDBY

	MOVLW 0x10		            	;BRON= h10  vraag gegevens op en stuur deze naar MASTER
	MOVWF BRON
	MOVFF DEVICENUMMER, DEVICENR	;van DEVICE waar toetsenbord op staat aangesloten
	MOVLW 1				            ;van poort 1
	MOVWF POORTNR
    MOVLW 1			
	MOVWF ADRESNR			        ;van adres 1
	
	CALL CODEER_NAAR_TE_VERZENDEN_BYTES

	BSF STANDBY			            ;plaats in STANDBY
	RETURN


	;**********************************************************************
	
;STANDBY: kunnen de volgende toetsen tot actie leiden:

;  DEVICE invoerveld kan enkel bereikt worden via toets #!!!

;  # --> CURSOR naar veld om DEVICE adres in te voeren, DEVICE invoerveld kan enkel bereikt worden via toets #!!!

;  * --> CURSOR naar veld om POORT Nr in te voeren

;  F1 --> geef Display met adres weer CURSOR gaat naar invoerveld ADRES

;  F2 --> geef Display met instellingen weer (indien van toepassing, en afhankelijk van FUNCTIE)
;         als FUNCTIE = ON/OFF zal elke F2 een adres opschuiven en de data ervan weergeven tot A12; F1 schuift terug
;         CURSOR gaat naar eerste invoerveld lijn2

;  >  --> schuif CURSOR een invoerveld rechts, indien einde balk spring naar onderste balk (indien van toepassing)

;  <  --> schuif CURSOR een invoerveld links, indien einde balk, spring naar balk omhoog (indien van toepassing)

;  /\ en \/  --> verhoog of verlaag waarde veld waar cursor op staat, op dit moment is de invoer enkel mogelijk via /\ en \/ (PIJL_INVOER)

;  0-9  --> voer nieuwe waarde in invoerveld nummeriek, op dit moment is de invoer enkel nummeriek mogelijk (NUM_INVOER)



;STANDBY =1

;    F1 F2 #  *
;    1  2  3  /\
;    4  5  6  \/
;    7  8  9  -
;    <  0  >  - 

STANDBY_SPR

	


	;**********************************************************************

;PIJL INVOER: Waarde kan hoger of lager per stap ingesteld worden (tot limiet)

;  /\  --> verhoog waarde (tot bovenste limiet)

;  \/  --> verlaag waarde (tot onderste limiet)

;  Esc --> verlaat PIJL INVOER, de vorige waarde wordt terug geplaats  daarna STANDBY =1

;  Ent --> Waarde wordt opgeslagen    daarna STANDBY =1  


;PIJL_INVOER =1

;    -  -  -  -   
;    -  -  - /\   
;    -  -  - \/   
;    -  -  - Esc   
;    -  -  - Ent  

PIJL_INVOER_SPR



	;**********************************************************************

;NUM_INVOER: waarde wordt nummeriek ingevoerd

;  0 ... 9 --> Bij elke toetsaanslag schuift waarde 1 positie naar links tot aantal posities (2, 3 of 4) dan is geen invoer meer mogelijk

;  Esc --> verlaat NUM INVOER, de vorige waarde wordt terug geplaats  daarna STANDBY =1

;  Ent --> Waarde wordt opgeslagen volgens onder en boven limieten   daarna STANDBY =1   


;NUM_INVOER =1

;    -  -  -  -                   
;    1  2  3  -         
;    4  5  6  -
;    7  8  9 Esc
;    -  0  - Ent

NUM_INVOER_SPR





	RETURN

