#include <LPC17xx.h>

int zustand = 0;
int richtung = 0;
// int extint_triggered = 0;
int timer1_triggered = 0;
int exint_tigerred = 0;
int led_go = 0;
int prelled = 0;

void init(void);
void TIMER2_IRQHandler(void);
void TIMER1_IRQHandlers(void);
void EINT0_IRQHandler(void);

int main(void){
	
	init(); //als erstes initialisieren aufrufen;

	while(1){//endlosschleife
		if(exint_tigerred){
			if(timer1_triggered && (zustand==0)){						//Taster drücken && Licht aus 
				zustand = 1;
				timer1_triggered =0;
				LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & 0xFF00;			//1111 1111 0000 0000 -> Pin 11 - 19 aktiv 
				LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN | 0x0001;			//0000 0000 0000 0001 -> Pin 0 aktiv 
			}
			else if(timer1_triggered && (zustand == 1)){				//Taster drücken && Licht an 
				zustand = 0; 
				timer1_triggered = 0;
				LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & 0xFF00; 		// alle LEDs aus
			}
			else if(!timer1_triggered && (zustand == 1) && !prelled){	// Taster nicht && Licht läuft weiter && nicht prelled 
			if(richtung == 0){
			richtung = 1;
			}
			else 
				richtung = 0;
			}
		}
		exint_tigerred =0;
	} //end of while(1)
		
	if((zustand == 1) && (richtung == 0)){								//Licht an, Richtung aus 
	LPC_TIM2->TCR |= 0x1;												//Timer Control Register -> Timer2 config? Bit 0 = 1
																		// -> 01 Counter Mode: TC is incremented on rising edges on the CAP input selected by bits 3:2.
	
	while(!(led_go)){													//wenn LED aus ( nicht led_go) -> Timer2 starten 
	continue; 															// TIMer 2 starten
	}
	if(LPC_GPIO2->FIOPIN & 0x80){										// 1000 0000 -> Bit 7 = 1 - logisches und (AND) 
																		// 7. LED an
		LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & 0xFF00; 				// alle LEDS aus und andere Werte beibehalten
		LPC_GPIO2->FIOPIN = LPC_GPIO0->FIOPIN | 0x0001; 				// 0. LED an
		led_go = 0;														// While Abbruch 
	}
	else{
	volatile int temp_fiopin = LPC_GPIO2->FIOPIN & 0x00FF;				// 0000 0000 1111 1111 (wenn Bit 0-7 = 1, dann weiterhin 1)
																		// volatile := bei jedem Aufruf Wert kontrolliert
		LPC_GPIO0->FIOPIN = temp_fiopin <<1;							// Wert in GPIO0 gespeichert, Wert aus temp_fiopin um 1 nach links 
		led_go=0;
	}
}
	else if((zustand ==1) && (richtung == 1)){							// taster gedrückt und richtung 1 (1 sollte nach links sein) 
	LPC_TIM2->TCR |= 0x1; 												//TIM2 starten								
		while(!led_go){
		continue;
		}
	
		if(LPC_GPIO0->FIOPIN & 0x1){									// 0001 -> Bit 0 an ist 
																		//1. led an
			LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & 0xFF00;
																		// alle LEDs aus und andere Werte beibehalen 
			LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN | 0x0080;
																		//7. led an
			led_go = 0;
	}
		else{
		volatile int temp_fiopin = LPC_GPIO2->FIOPIN & 0x00FF;
			LPC_GPIO2->FIOPIN = temp_fiopin >> 1;						// Wert in GPIO0 gespeichert, Wert aus temp_fiopin um 1 nach rechts 	
			led_go = 0;
		}
	}
}
	



	
	
void init(void){ 						// Initialisieren
	LPC_GPIO2->FIODIR = 0xFF; 			// P2.0 bis 2.8 als Ausgang
	LPC_GPIO2->FIOPIN = 0x00; 			// LED ausschalten
	LPC_TIM1->MCR |= (7<<0); 			// Interrupt T1 wenn Wert erreicht & stoppen und zuruecksetzen auf MR0 Wert,	
										// 7 bits auf 0 setzen und 0 nach links shiften
	LPC_TIM2->PR = (25-1); 				// Prescale Register, Timer 2
	LPC_TIM2->MR0 = (12500-1); 			// LED timer 0.125s, hochzaehlen bis 7 dann 0
	NVIC_EnableIRQ(TIMER1_IRQn); 		// Interrrupt Timer 1 aktiv
	NVIC_SetPriority(TIMER1_IRQn,1); 	// Priorität Interrupt Timer 1
	NVIC_EnableIRQ(TIMER2_IRQn); 		// Interrupt Timer2 
	NVIC_SetPriority(TIMER2_IRQn,1); 	// Interrupt Priorität
	LPC_SC->EXTMODE |= (1<<0); 			// Edge sensitive mode on EINT0
	LPC_SC->EXTPOLAR &= ~(1<<0); 		// falling edge sensitive
	LPC_PINCON->PINSEL4 &= ~(3<<20); 
	LPC_PINCON->PINSEL4 |= (1<<20);
	NVIC_EnableIRQ(EINT0_IRQn);			// Nested vectored interrupt controller
	NVIC_SetPriority(EINT0_IRQn,2); 	// Prio setzen
	return;
}
void TIMER2_IRQHandler(void){
	//Flag clearen vom Interrupt
	LPC_TIM2->IR = 1;
	led_go = 1;
	return;
}
void TIMER1_IRQHandlers(void){
	LPC_TIM1->IR = 1; //Flag clearen
	timer1_triggered = 1; // name verändern
	return;
}
void EINT0_IRQHandler(void){
	if((LPC_SC->EXTPOLAR & 0x1) ==0 ){
		//wenn fallende Flanke da extpolare
	 prelled = 0; // umbenennen
		LPC_TIM1->TCR |= 0x2; // timer 1 reset 0x2??
		LPC_TIM1->TCR &= ~(1<<1); //1 um 1 nach links shift, tcr?
		LPC_TIM1->TCR |= 0x1; // Timer 1 starten
	}
	else{
		if(LPC_TIM1->TC <= 1000){
			//taste kuerzer als 10ms 
			prelled = 1;
		}
		else{
			prelled = 0;
		}
		LPC_TIM1->TCR &= ~(0x1); // TImer stoppen
		LPC_TIM1->TCR |= 0x2; // TIMER reset
		LPC_TIM1->TCR &= ~(1<<2);
		exint_tigerred = 1;
	}
	LPC_SC->EXTPOLAR ^= (1<<0); // POlaritaet aender
	LPC_SC->EXTINT |= 1<<0; // flag clearen
	return;
}