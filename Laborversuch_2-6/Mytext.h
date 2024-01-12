/******************************************************************************/
/***********       Text to be sent on UART Port   *****************************/
/******************************************************************************/
char *MyText = "Test UART return";

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

*/
