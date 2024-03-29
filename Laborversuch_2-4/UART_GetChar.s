	THUMB ; Directive indicating the use of UAL
	AREA Code1, CODE, READONLY, ALIGN=4

	INCLUDE	LPC1768.inc
	EXPORT 	UART_GetChar
	;Assumption R0 = UART_PortNum (0 or 2 for LandTiger EVB)

UART_GetChar	PROC
;LSR: RDR lesen
;wenn gesetzt, RBR lesen
	STMFD	SP!,{R4, R5, LR}		; STMFD, STMIA Rn{!}, reglist Store Multiple registers, increment after -
									; SP=StackPointer, !=wird so bei bearbeitung immer akitalisiert -> R4, R5, LR aus Stack 
									; Sicherung der genannten Register, dass diese f�r andere Funktionalit�ten
									; nicht verloren gehen -> am Ende wieder Laden
	; R0 aus Funktionsaufruf in C
	ANDS R0, R0, #0xFF				; Abfrage ob UART0
	BEQ UART_Port0

UART_Port2							; Port 2 selected
	LDR R4, = U2LSR
	LDR R4, [R4]
	ANDS R4, R4, #0x01				; 0000 0001 -> Bit 0 f�r Receiver Data Ready: 1=not empty, 0=empty
	BEQ No_Data
	LDR R5, =U2RBR
	LDR R0, [R5]
	B UART_Rx_Done
	
UART_Port0							; Port 0 selected
	LDR R4, = U0LSR
	LDR R4, [R4]
	
	ANDS R4, R4, #0x01				; Abfrage gesetzt?
	BEQ No_Data						; nichts drin -> reset
	LDR R5, = U0RBR
	LDR R0, [R5]
	B UART_Rx_Done
	
No_Data
	LDR R0, =0xFFFFFFFF				; -1

UART_Rx_Done
	LDMFD	SP!,{R4, R5, PC}		;LDMFD, LDMIA Rn{!}, reglist Load Multiple registers, increment after -

	ENDP

	END
