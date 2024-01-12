
#include <stdio.h>
#include <LPC17xx.H>  

extern void UART_init(int UART_PortNum,int U_DL,int UART_Mode,int INT_EN_FIFO_Mode);
extern char UART_GetChar(int port_nr);
extern void UART_PutChar(int port_nr, char symbol);

unsigned char MyChar;
unsigned int PortNum, UDL, FDRvalue, UART_mode, IntEnableValue, FIFO_mode, Temp;
// PortNum = Port Number excepted to be 0 or 2 for the the LandTiger EVB
// UDL = U_DLM*256 + U_DLL to set the data rate
// FDRvalue  = Content of the register UxFDR (Fractional Divider)
// UART_mode = Content of the register UxLCR without the DLAB bit
// IntEnableValue = Content of the register UxIER
// FIFO_mode = Content of the register UxFCR (FIFO Control Register)



int main(void){

	int PortNum = 0; 																// UART0 -> wieder hardcoding, wie "soft"coding?
																									// kann ich in getchar.s file einfach das R0 Reg ergänzen, sodass eine 0 oder 1 für UART Wahl senden kann?
	int UDL = 94; 																	// Baudrate
	int UART_mode = 3;
	int FIFO_MODE = 3; 															// 
	//Werte aus Beispielaufruf
	char MyChar;
		
	UART_init(PortNum, UDL, UART_mode, FIFO_MODE);	// vorgabe aufgabensgtellung
																									// C-Compiler verwaltet automatisch die Var
																									// in Reg, immer beginnend bei R0, dann R++
	
	if(PortNum == 0){
		LPC_UART0 -> FDR = 0x21; 											// 0010 0001 MulVal/Divaddval

	}
	else{
		LPC_UART2 -> FDR = 0x21; 
	}
	while(1){
		MyChar = UART_GetChar(PortNum);								// UART dem Mychar zuweisen
		if(MyChar != 0xFFFFFFFF)
			UART_PutChar(PortNum, MyChar);							// UART	
	}
}
// Beispielaufruf: UART_init(0, 94, 3, 3); 
//UART Port 0, Baudrate 9600 Bit/s, 8 Bits, No Parity, 1 
//Stop Bit, FIFO enabled, RX FIFO cleared
