	THUMB ; Directive indicating the use of UAL
	AREA Code1, CODE, READONLY, ALIGN=4

	INCLUDE	LPC1768.inc
	IMPORT UART_init
	IMPORT UART_PutChar

	EXPORT __main
	ENTRY
__main	PROC			; LABORVERSUCH 2-1
	; Configure UART
	LDR	R0, = 0x00		; UART_PortNum				; 0 or 2
	LDR	R1, = 0x09		; UxDL  = (UxDLM << 8) | (UxDLL)  - 100Mhz/(16*115200bit/s)=54,25347222
	LDR	R2, =0x3		; (UxFDR (7:4 MULVAL  3:0 DIVADDVAL) <<8) | UxLCR (UART_Mode) ):
						;	DLAB 0  Set Break 0  Stick Parity 0  Even Parity Select 0  
						;	Parity Enable 0  Number of Stop Bits 0  Word Length Select 11
	LDR	R3, =0x0007		; (UxIER << 8) | UxFCR)		; TX FIFO Reset 1 ; RX FIFO Reset 1 ; FIFO Enable 1

	; R0 = UART_PortNum, R1 = U_DL , R2 = (UxFDR (7:4 MULVAL  3:0 DIVADDVAL) <<8) | UxLCR (UART_Mode), R3 = (INT_ENABLE << 8) | FIFO_Mode
	; 8 Nutzdatenbits pro Sendung, keine Parität, 1 Stopp Bit ; Interrupts ausgeschaltet
	; FIFO aktiv, Trigger level = 1

	MOV R4,R0			; Copy of PortNum (weil R0..R3 Scratch-Register sind, -> in einer gerufenen Funtion verändert werden können!)
	BL 	UART_init
	LDR R5, =MyString	; Laden String in R5
	
	LDR R0, = 0x00		; R0 nochmal auf 0 - wurde in UART_init verwendet -> falscher Wert drin
	LDR R8, = 0x00		; starte mit Byte 0 von string

Read_String				; MainLoop
; PUT YOUR CODE HERE:

	LDRB R1,[R5,R8] 		; Load Byte von R5(Mystring) mit offset R8 in R1
	CBZ R1, FinishLoop  ; compare, branch if zero 

	BL UART_PutChar	 	; Springe zu putchar - Ausgabe
	
	ADD R8, R8, #0x01  	; hochzählen der Byte variable
	

	B Read_String
	
FinishLoop	
	B	.	; End Of Program (Sprung auf sich selbst)

	ENDP	; End Procedure

	AREA	String_Block, DATA, READONLY
MyString	DCB  "Hello there!",10,0		; 10 = LF (Line Feed) for new line, 0 = End of String

	END