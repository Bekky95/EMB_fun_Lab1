/******************************************************************************/
/***********       Text to be sent on UART Port   *****************************/
/******************************************************************************/
char *MyText = "Hoffentlich grillt mein Code die LCD <3 \n";

/*
* UART Modul ist die RX Interrupt Leitung standardmäßig freigeschaltet. 
* empfangene Zeichenkette wird in den Zeilen 3 bis 8 am Display Modul 
* - 20 Zeichen pro Zeile
* Text wird 10 Sekunden lang angezeigt
* Ein längerer Text wird in Teile von maximal 20 Zeichen zerlegt, so 
* dass diese Teile der Reihe nach jeweils nach vorgegebenem 
* Zeitabstand am LCD Display angezeigt werden
* Eine neue Nachricht muss mit Character 0x0A (LineFeed) beendet sein
* UART Sendevorgang wird mit dem Taster INT0 ausgelöst - weiteren 
* Zeichen werden per TX Interrupt gesendet
* Ihre Aufgabe besteht nun darin, die Konfiguration des Interrupt 
* Controllers (NVIC) sowie die Initialisierung des Timer 0 Moduls
* 		void init_NVIC(void)
* 		void init_Timer0_for_UART(void)
* -> Diese zwei Funktionen können am Ende der Datei UART_api.c eingefügt werden

void  init_Timer0_for_UART(void) {
	//Konfiguration von Timer0
	LPC_SC->PCONP |= (1<<1);									// Timer0 anschalten, PCONP.1 auf 1 
	LPC_TIM0->PR = 0x2500000-1;									// Taktfrequenz
	
	LPC_TIM0->MR0  = 0x10;										// nach Ablauf 10 sek
	LPC_TIM0->MCR |= 3;												// Bit0 = 1 = Interrupt an ; Bit1 = 1 = Reset bei Interrupt
	LPC_TIM0->TCR |= 3;												// Timer und Interrupt angeschalten um zu zählen, Bit1 positive Flanke zählen
	LPC_TIM0->TCR &= ~(1<<1);									// Bit1 = 0,  Counter reset

}

void init_NVIC(void){
	NVIC_EnableIRQ(TIMER0_IRQn);							// Interrrupt für Timer0
	NVIC_SetPriority(TIMER0_IRQn, 3);					// Prio 3
	NVIC_EnableIRQ(UART0_IRQn);								// Interrupt für UART0
	NVIC_SetPriority(UART0_IRQn, 2);					// Prio 2
}
*/