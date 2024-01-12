	THUMB ; Directive indicating the use of UAL
	AREA Code1, CODE, READONLY, ALIGN=4

	INCLUDE	LPC1768.inc

	IMPORT UART_init
	IMPORT UART_PutChar
	IMPORT UART_GetChar

	EXPORT __main
	ENTRY
	
__main	PROC
	
	LDR R0, =0x00			; UART_PortNum0 -> muss man vorher immer anpassen, je nachdem welcher UART 0=0x00, 2=0x01
	LDR R1, =0x09			; UxDL Datenrate
	LDR R2, =0x03			; -> UART_Mode
	LDR R3, =0x41			; FIFO Mode (INT_ENABLE << 8) - 0100 0001
	LDR R5, =0x00

	BL UART_init
	
main_loop
	BL UART_GetChar			; Fkt-Aufruf
	
	CMP R0, #0xFFFFFFFF		; Abgleich ob -1 (No_Data) oder Wert zurück
	MOVNE R1, R0			; Buchstabe in R1
	MOVNE R0, R5			; Welcher Port -> R5
	
	LDR R0, =0x00			; 0x00 für UART0, sonst. für UART2
	BLNE UART_PutChar		; Funktionsaufruf gibt dann aus
	BNE main_loop
	B main_loop
	
main_done
	B main_loop

	ENDP	; End Procedure

	END