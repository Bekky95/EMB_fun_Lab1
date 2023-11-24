#include <LPC17xx.h>
#include <stdbool.h>
#include "GLCD.h"

bool zustand = false;
bool richtung = false;													// f= links, t= rechts 
int fromTasterCounter;
int tmpLED = 0x01;   //0x0 gibt es nicht
// für LCD:
int LCDupdate = 0;


void Init(void);
void TIMER2_IRQHandler(void);
void TIMER1_IRQHandlers(void);


int main(void){
	Init();
	//LCD: erste beiden Zeilen ausgeben
	GLCD_DisplayString(0, 0, "EMB-Lab1 Lauflicht");/* print string to LCD */
	GLCD_DisplayString(1, 0, "GPIO/Timer");/* print string to LCD */
	while(true){
		
		if(LCDupdate == 1){						// 1: etwas geändert
			if(!zustand){								// LEDs aus
				//LCD : LEDs OFF  // Zeile 3 links beginnend 
				GLCD_DisplayString(3, 0, "LEDs OFF           ");/* print string to LCD */
			}
			else {											// LEDs an
				if(richtung){							// nach links
					//LCD : LEDs ON LEFT		// Zeile 3 links beginnend 
					GLCD_DisplayString(3, 0, "LEDs ON LEFT     ");/* print string to LCD */
				}
				else {										// nach rechts
					//LCD: LEDs ON RIGHT		// Zeile 3 linnks beginnend
					GLCD_DisplayString(3, 0, "LEDs ON RIGHT    ");/* print string to LCD */
				}
			}
			LCDupdate = 0;
		}
		// wenn LCDupdate = 0 -> einfach weiter 
		
	} // end of while(true) 
} //end of main()

// -----------------------------------------------------------------------------------	

void Init(void){

	// PCONP -> Peripheral Power Register, User man S.65
	LPC_SC->PCONP |= (1 << 2);			// Timer 1 einschalten - Bit 2
	LPC_SC->PCONP |= (1 << 22);   	// Timer 2 einschalten - Bit 22

	// GPIO -> 1 out, 0 in 
	LPC_GPIO2->FIODIR = 0xFF; 			// P2.0 bis 2.8 als Ausgang
	LPC_GPIO2->FIOPIN = 0x00; 			// LED ausschalten
																	
	// Timer 1 -> für stufenweise Abfrage Taster
	LPC_TIM1->MCR |= (3 << 0);			// MCR einschalten Timer 1: if val tmr == max_val -> reset
	LPC_TIM2->MCR |= (3 << 0);
	LPC_TIM1->TCR |= 0x1; 						// Timer 1 starten
	LPC_TIM1->PR = (25000-1);					// korreliert mit den 25MHz 
	LPC_TIM1->MR0 = (100-1);					// -> 100ms -> Länge Schritt zum Abtasten  NOCHMAL MAL 10 FÜR HW
																	
	// Timer 2 -> für LED-Lauf
	LPC_TIM2->PR = (25-1); 					// Prescale Register, Timer 2 -> max-val
	LPC_TIM2->MR0 = (125000-1); 			// LED timer 0.125s, hochzaehlen bis 7 dann 0 (Interrupt wenn val erreicht)   FÜR SIM VERÄNDERT (12500-1)
	LPC_TIM2->TCR |= 0x1; 						// Timer 2 starten																
	// Interrupt Handling (Skript 5-58) 
	NVIC_EnableIRQ(TIMER1_IRQn);		// Interrupt Timer 1 aktivieren -> stufenweise Abnahme in 100ms Schritten -> TR > <= 20?
	NVIC_SetPriority(TIMER1_IRQn,1);// Priorität Interrupt Timer 1 = 1
																	// user man S.777 interrupt -> enable, priority
	NVIC_EnableIRQ(TIMER2_IRQn); 		// Interrupt Timer 2 aktivieren
	NVIC_SetPriority(TIMER2_IRQn,1);// Priorität Interrupt Timer 2 = 1
																	
	// PINSEL-Reg user_manual S.114 Übersicht welcher Port in welchem PINSEL-Reg
	LPC_PINCON->PINSEL4 &= ~(3<<20);// val reset in Bit 21:20 and 0b11 -> P2.10 (Taster)

	//LCD:
	GLCD_Init();
	GLCD_Clear(White);
}	//end of Init


void TIMER2_IRQHandler(void){			// Skript 5-109 && 5-169
	LPC_TIM2->IR = 1;								// 1:  Interrupt-flag clear
	//led_go = true;
	if(zustand){										// LED eingeschaltet?
		if(richtung){									// Richtung true - nach links
			if (tmpLED == 0x80){				// LED auf 2.8?
				tmpLED = 1;
			}
			else{												// LED nicht auf 2.8 - Durchlauf
			tmpLED = (tmpLED << 1);
			}
		} // end of if(richtung)
		else {												// Richtung false - nach rechts
			if (tmpLED == 0x01){				//LED auf 2.0
				tmpLED = 0x80;
			}
			else{												// LED nicht auf 2.0 - Durchlauf
			tmpLED = (tmpLED >> 1);
			} //end of else (-> !richtung)
		}
		LPC_GPIO2->FIOPIN = tmpLED;		// Val auf Port laden
	} // end of if(zustand)
		else{													// LED nicht eingeschaltet
			LPC_GPIO2->FIOPIN = 0x0;		// -> alles aus
		} //end if else (! zustand)
} // end of TIMER2_IRQHandler


void TIMER1_IRQHandler(void){			
	LPC_TIM1->IR = 1; 							// 1:  Interrupt-flag clear
	if (LPC_GPIO2->FIOPIN & 0x400){	// 0100 0000 0000 -> Bit10?
		// wenn an Bit 10 eine 1 steht: Counter = 0
		if(fromTasterCounter > 0){
			LCDupdate = 1;  // wenn hier 1, wird in richtung und zustand geschaut, ausgegeben und LCDupdate wieder = 0
			if(fromTasterCounter >= 20){ // abfrage mit 2sek
				zustand ^= 1;							// Invertieren
			}
			else{
				richtung ^= 1; 						//Invertieren 
			}
			fromTasterCounter = 0;
		} // end of if(fromTasterCounter > 0)	
	} // end of if (LPC_GPIO2->FIOPIN & 0x400)
	else {
		// wenn Tastersignal = 0, Counter++
		fromTasterCounter+=1;
	} // end of else (-> ! (LPC_GPIO2->FIOPIN & 0x400))
} // end of TIMER1_IRQHandler
