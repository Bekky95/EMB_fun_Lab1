#include <LPC17xx.h>
#include <stdbool.h>

bool zustand = false;
bool richtung = false;													// f= links, t= rechts 
bool timer1_triggered = false;
bool taster_pressed = false;
bool led_go = false;
bool prelled = false;

void Init(void);
void TIMER2_IRQHandler(void);
void TIMER1_IRQHandlers(void);
void EINT0_IRQHandler(void);

// HABE ZUSTANDVARIABLEN VON INT ZU BOOL GEÄNDERT

int main(void){
	
	Init();
	
	while(true){																	// Endlos-Loop
		if(taster_pressed){													// Taster gedrückt? -> EINT0 Interrupt
			if(timer1_triggered && ! zustand){				// Timer 1 interrupt and LED aus?
				zustand = true;													// -> LED an
				timer1_triggered = false;								// -> var-reset timer1_triggered
																								// Pin clear:
																								// 1111 1111 0000 0000 -> Pin 11 - 19
				LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & 0xFF00;
																								// Pin0 activate:
																								// 0000 0000 0000 0001 -> Pin 0 aktiv 
				LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN | 0x0001;
			} // end of if(timer1_triggered && ! zustand)
			
			else if(timer1_triggered && zustand){			// Timer 1 interrupt and LED an?
				zustand = false;												// -> LED aus
				timer1_triggered = false;								// -> var-reset timer1_triggered
																								// Pin clear: -> aus
																								// 1111 1111 0000 0000 -> Pin 11 - 19
				LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & 0xFF00;
			} // end of if(timer1_triggered && zustand)
			
			else if (! timer1_triggered && zustand && ! prelled){	
																								// Kein Timer 1 Interrupt and LED an?
																								// -> Richtungswechsel
				if(! richtung){												 
					richtung = true;											// nach rechts
				}
				else{
					richtung = false;											// nach links
				}
			} // end of if (! timer1_triggered && zustand)			
		} // end of if(taster pressed)
		
		taster_pressed = false;											// reset var
		
																								// LED-Wechsel:
		if (zustand && ! richtung){									// LED an und nach links?
			LPC_TIM2->TCR |= 0x01;										// Timer 2 starten -> LED Lauf
			while(! led_go) {continue;}								// solange kein timer2 Interrupt (0.125 ms nicht abgelaufen) 
																								// geht zu while 1 bis Timer2 abgelaufen 			
			if(LPC_GPIO2->FIOPIN & 0x80){							// 2.8 = 1? -> letzte LED an?
				LPC_GPIO2->FIOPIN &= 0xFF00;						// 2.8 auf 0 (2.7- 2.0 zur Sicherheit auch)
				LPC_GPIO2->FIOPIN |= 0x01;							// 2.0 = 1 -> erste LED an
				led_go = false;													// var-reset
			} // end of if(LPC_GPIO2->FIOPIN & 0x80)
			else {																		// Durchlauf von 2.0 bis 2.8
				int tmpLED = LPC_GPIO2->FIOPIN & 0xFF;	// Speichern welche LED gerade leuchtet
				LPC_GPIO2->FIOPIN = tmpLED << 1;				// LED += 1 -> nächste
				led_go = false;													// var-reset
			} // end of else
		} // end of if (zustand && ! richtung)
		
		else if(zustand && richtung){								// LED an und nach rechts?
			LPC_TIM2->TCR |= 0x01;										// Timer 2 starten -> LED Lauf
			while(! led_go){continue;}								// solange kein timer2 Interrupt (0.125 ms nicht abgelaufen) 
																								// geht zu while 1 bis Timer2 abgelaufen 
			if(LPC_GPIO2->FIOPIN & 0x01){							// 2.0 = 1? -> erste LED an?
				LPC_GPIO2->FIOPIN &= 0xFF00;						// 2.0 auf 0
				LPC_GPIO2->FIOPIN = 0x80;								// 2.8 = 1 -> letzte LED an
				led_go = false;													// var-reset
			}
			else{																			// Durchlauf 2.8 bis 2.0
				int tmpLED = LPC_GPIO2->FIOPIN & 0xFF;	// Speichern welche LED gerade leuchtet
				LPC_GPIO2->FIOPIN = tmpLED >> 1;				// LED -= 1 -> nächste
				led_go = false;													// var-reset
			} // end of else
		} // end of if(zustand && richtung)
				
	} //end of endlos-loop

} //end of main

// -----------------------------------------------------------------------------------	

void Init(void){
	SystemInit();										//init von System
	
	LPC_SC->PCONP |= (22 << 1);   	// Timer 2 via PCON anschalten - Bit 22
																	// PCONP -> Peripheral Power Register, User man S.65
																	// -> Bit 22 einschalten 	LPC_SC_TypeDef->PCONP |= (22<<1) -> Timer2
																	// Timer1 ist default auf 1
																	// GPIO -> 1 out, 0 in 
	LPC_GPIO2->FIODIR = 0xFF; 			// P2.0 bis 2.8 als Ausgang
	LPC_GPIO2->FIOPIN = 0x00; 			// LED ausschalten
																	// Timer 1 -> für LEDs
	LPC_TIM1->MCR |= (7 << 0);			// MCR einschalten Timer 1: if val tmr == max_val -> reset
																	// Timer 2 -> Prellung
	LPC_TIM2->PR = (25-1); 					// Prescale Register, Timer 2 -> max-val
	LPC_TIM2->MR0 = (12500-1); 			// LED timer 0.125s, hochzaehlen bis 7 dann 0 (Interrupt wenn val erreicht)
																	// Interrupt Handling (Skript 5-58) 
	NVIC_EnableIRQ(TIMER1_IRQn);		// Interrupt Timer 1 aktivieren 
	NVIC_SetPriority(TIMER1_IRQn,1);// Priorität Interrupt Timer 1 = 1
																	// user man S.777 interrupt -> enable, priority
	NVIC_EnableIRQ(TIMER2_IRQn); 		// Interrupt Timer 2 aktivieren
	NVIC_SetPriority(TIMER2_IRQn,1);// Priorität Interrupt Timer 2 = 1
	
	LPC_SC->EXTMODE |= (1<<0); 			// Edge sensitive mode (Flanken) on EINT0 ->  EINT0 verbunden mit P2.10
	LPC_SC->EXTPOLAR &= ~(1<<0); 		// auf fallende Flanke
																	// PINSEL-Reg user_manual S.114 Übersicht welcher Port in welchem PINSEL-Reg
	LPC_PINCON->PINSEL4 &= ~(3<<20);// val reset in Bit 21:20 and 0b11 -> P2.10 (Taster)
	LPC_PINCON->PINSEL4 |= (1<<20);	// set GPIO Port 2.10 (20) auf 1
	
	NVIC_EnableIRQ(EINT0_IRQn);			// Nested vectored interrupt controller
	NVIC_SetPriority(EINT0_IRQn,2); // Prio setzen
	return;
}	//end of Init


void TIMER2_IRQHandler(void){			// Skript 5-109 && 5-169
	LPC_TIM2->IR = 1;								// 1:  Interrupt-flag clear
	led_go = 1;
	return;
}

void TIMER1_IRQHandlers(void){
	LPC_TIM1->IR = 1; 							// 1:  Interrupt-flag clear
	timer1_triggered = 1; 					// name verändern
	return;
}


void EINT0_IRQHandler(void){
	if((LPC_SC->EXTPOLAR & 0x1) == 0 ){ 
																	// (Bit0: 1 wenn 1) ist es 0? 0 = fallende Flanke
																	// The External Interrupt Polarity Register controls
																	// which level or edge on each pin will cause an interrupt
																	// wenn fallende Flanke da extpolare
		prelled = false; 							// 
		LPC_TIM1->TCR |= 0x2; 				// timer 1 reset 0x2 = 0b10 -> Notiz im Skript: 2 Reg
		LPC_TIM1->TCR &= ~(1<<1); 		// 1 um 1 nach links shift, clear
																	// TCR = Timer Control Reg - Counter 0: enable, 1: reset
		LPC_TIM1->TCR |= 0x1;					// Timer 1 starten 0x1 = 0b01 
	}
	else{														// steigende Flanke
		if(LPC_TIM1->TC <= 1000){			// Taste kürzer als 10ms gedrückt
			prelled = true;
		}
		else{													// Taste länger als 10ms gedrückt
			prelled = false;
		}
		// zu steigende Flanke Routine
		LPC_TIM1->TCR &= ~(0x1); 			// Timer 1 stoppen
		LPC_TIM1->TCR |= 0x2; 				// Timer 1 reset
		LPC_TIM1->TCR &= ~(1<<2);			// Timer 1 clear
		taster_pressed = true;
	}
	
	LPC_SC->EXTPOLAR ^= (1<<0); 		// Polaritaet ändern, Stelle 0 = 1 -> rising edge sensitive
	LPC_SC->EXTINT |= 1<<0; 				// flag clear
	return;
}