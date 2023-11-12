	AREA Code1, CODE, READONLY
	
	INCLUDE LPC1768.inc
	EXPORT __main
	ENTRY
	
__main  PROC
			
init		
	
			LDR R1, =FIO2DIR0   ; Pin 0 - 7 Port 2 -> R1
			LDR R2, =FIO2DIR1   ; Pin 8 - 15 Port 2 -> R2
			
			LDR R3, =0xFF       ; 1111 1111 -> R3
			LDR R4, =0x00       ; 0000 0000 -> R4
			
			STR R3, [R1]        ; Inhalt R3 (0xFF) an Adresse von R1 speichern
								; -> Port 2.0 - 2.7 auf Ausgang (LED)
			STRB R4, [R2]       ; Store Byte von R4 (0x00) an Adresse von R2 
								; -> Port 2.8 - 2.15 auf Eingang (Taster)
			LDR R3, =FIO2PIN0   ; 0 -> alle Werte in Port 2.0 - 2.7 sind low, 
								; -> damit auch 2.2 (LED) val = 0
			LDR R4, =FIO2PIN1   ; 1 -> alle Werte in Port 2.8 - 2.15 sind high
								; -> damit auch 2.10 (Taster) val = 1

main 
			LDR R6, [R4]        ; Wert aus R4 in R6 speichern
			TST R6, #0x04       ; 0000 0100 -> Pin 10 (Taster)
								; -> Testen ob Taster gedrückt wurde
			BNE main            ; Branch if not equal (Taster nicht gedrückt)
			LDR R0, =2500000    ; 2.500.000ms delay -> "Software-Prellen"
			BL delay
			
set			
			LDR R6, [R4]		; Taster abrufen
			TST R6, #0x04		; Taster noch gedrückt?
			BNE main			; Gedrückt: läuft weiter, ungedrückt zurück in main
			
ng								; wieder loslassen
			LDR R6, [R4]
			TST R6, #0x04		; Taster noch gedrückt?
			BEQ ng				; Noch gedrückt 
			LDR R7, [R3]		; R3 (Werte der LED) auf R7
			EOR R7, R7, #0xF8	; R7 EXOR 1111 1000 -> 
								; EXOR: (0, 0):0, (0, 1):1, (1, 0):1, (1, 1):0
								; -> 2.0 - 2.2 auf 1 -> LED an 
			STR R7, [R3]		; geänderte Werte wieder in den Val von LED
			B main

delay       
			SUBS R0, R0, #1		; R0-- -> Delay herunterzählen
			BNE delay
			B set
			

			END	
		