#include <stdio.h>
#include <stdbool.h>
#include "LPC17xx.h"
#include "GLCD.h"
#include "touch.h"
#include "funk.h"
int LED[] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};
int x = 0;
int n = 1;

	void LED_init(void){
		LPC_GPIO2->FIODIR |= 0x000000ff;  //P2.0...P2.7 Output LEDs on PORT2 defined as Output
	};

	void interface(){
		
		LCD_Clear(White);
		GUI_Text(50,0, "Synchrones_Lauflicht",Green, Blue);
		
	};
	void Display_LED(){
	//LCD_DrawLine(  x0,   y0,   x1,   y1);
	LCD_DrawLine(  30,  30,   30,  190, Black);//Vertikal liniks
	LCD_DrawLine(  30,  30,   50,   30, Black);//Horizontal oben
		LCD_DrawLine(  30,  50,   50,   50, Black);//Horizontal 1
		LCD_DrawLine(  30,  70,   50,   70, Black);//Horizontal 2
		LCD_DrawLine(  30,  90,   50,   90, Black);//Horizontal 3
		LCD_DrawLine(  30, 110,   50,   110, Black);//Horizontal 4
		LCD_DrawLine(  30, 130,   50,   130, Black);//Horizontal 5
		LCD_DrawLine(  30, 150,   50,   150, Black);//Horizontal 6
		LCD_DrawLine(  30, 170,   50,   170, Black);//Horizontal 7
	LCD_DrawLine(  30, 190,   50,  190, Black);//Horizontal unten
	LCD_DrawLine(  50,  30,   50,  190, Black);//Vertikal rechts
	
	}
	
	void Display_LED_an(int x){
		int i = 0;
		
		if(x==0){
			
					for(i = 1; i < 20; i++) 
						//LCD_DrawLine(  x0,   y0,   x1,   y1);
							LCD_DrawLine(  31,  190-i,  49,  190-i, Blue);//Horizontal oben
				
					}
		else if(x==1){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  170-i,  49,  170-i, Blue);
		}
		else if(x==2){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  150-i,  49,  150-i, Blue); 
		}
		else if(x==3){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  130-i,  49,  130-i, Blue); 	
		}
		else if(x==4){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  110-i,  49,  110-i, Blue); 
		}
		else if(x==5){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  90-i,  49,  90-i, Blue); 
		}
		else if(x==6){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  70-i,  49,  70-i, Blue); 
		}
		else if(x==7){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  50-i,  49,  50-i, Blue); 
		}
		else if(x==8){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  30-i,  49,  30-i, Blue); 
		}
				

	}
	
		void Display_LED_aus(int x){
		int i = 0;
		
		if(x==0){
			
					for(i = 1; i < 20; i++) 
						//LCD_DrawLine(  x0,   y0,   x1,   y1);
							LCD_DrawLine(  31,  190-i,  49,  190-i, White);//Horizontal oben
				
					}
		else if(x==1){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  170-i,  49,  170-i, White);
		}
		else if(x==2){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  150-i,  49,  150-i, White); 
		}
		else if(x==3){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  130-i,  49,  130-i, White); 	
		}
		else if(x==4){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  110-i,  49,  110-i, White); 
		}
		else if(x==5){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  90-i,  49,  90-i, White); 
		}
		else if(x==6){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  70-i,  49,  70-i, White); 
		}
		else if(x==7){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  50-i,  49,  50-i, White); 
		}
		else if(x==8){
					for(i = 1; i < 20; i++) 
							LCD_DrawLine(  31,  30-i,  49,  30-i, White); 
		}
				

	}
	
	
	void test_lcd(void){
		int i;	
		//int z=0;
		//for(z = 0; z<100; z++){
		while(1){
			LPC_GPIO2->FIOPIN |= 0x1;	
		Display_LED_an(0);
		//Display_LED_aus(7);
		for(i=0;i<1000000;i++)
			LPC_GPIO2->FIOPIN |= (1<<0x1); 
		Display_LED_an(1);
		//Display_LED_aus(0);
		for(i=0;i<1000000;i++)
			LPC_GPIO2->FIOPIN |= (2<<0x1); 
		Display_LED_an(2);
		//Display_LED_aus(1);
		for(i=0;i<1000000;i++)
			LPC_GPIO2->FIOPIN |= (4<<0x1); 
		Display_LED_an(3);
		//Display_LED_aus(2);
		for(i=0;i<1000000;i++)
			LPC_GPIO2->FIOPIN |= (8<<0x1); 
		Display_LED_an(4);
		//Display_LED_aus(3);
		for(i=0;i<1000000;i++)
			LPC_GPIO2->FIOPIN |= (16<<0x1); 
		Display_LED_an(5);
		//Display_LED_aus(4);
		for(i=0;i<1000000;i++)
			LPC_GPIO2->FIOPIN |= (32<<0x1);
		Display_LED_an(6);
		//Display_LED_aus(5);
		for(i=0;i<1000000;i++)
			LPC_GPIO2->FIOPIN |= (64<<0x1); 
			Display_LED_an(7);
			//Display_LED_aus(6);
			
			Display_LED_aus(6);
			Display_LED_aus(5);
	  	Display_LED_aus(4);	
			//Display_LED_aus(3);
		  Display_LED_aus(3);
			Display_LED_aus(2);
			Display_LED_aus(1);
			Display_LED_aus(0);
			Display_LED_aus(7);
		}
			
		LED_init();
		
	}
	
	
	void TIMER1_init(void) {
		int TeilungsfaktorTimer1 = 1;
//		switch(LPC_SC->PCLK_TIMER0) {
//		case 0:
//			TeilungsfaktorTimer1 = 4;
//		break;
//		case 1:
//			TeilungsfaktorTimer1 = 1;
//		break;
//		case 2:
//			TeilungsfaktorTimer1 = 2;
//		break;
//		case 3:
//			TeilungsfaktorTimer1 = 8;
//		break;
		
//	}
	TeilungsfaktorTimer1++;
	LPC_TIM1->MR0 = SystemCoreClock/TeilungsfaktorTimer1/5; // = 1s
	LPC_TIM1->MCR = 3;				/* Interrupt and Reset on MR0 */

	NVIC_EnableIRQ(TIMER1_IRQn);
	// Enable Timer
	LPC_TIM1->TCR = 1;
	}


void LED_an(int x){
	LPC_GPIO2->FIOPIN |= LED[x];
}

void LED_aus(int x){
	LPC_GPIO2->FIOPIN &= ~LED[x];
}

void Lauflicht(void){

		int safe=0;
	
	if(x==8)
		x=0;
	
	if(n==1)
	{
	LED_an(x);
		for (safe = 0; safe<100; safe++)
		{;}
	Display_LED_an(x);
	
	}		
	else 
	{
	LED_aus(x);
		for (safe = 0; safe<100; safe++)
		{;}
	Display_LED_aus(x);
	x++;
	}
	if(n==0)
	{
		n=1;
	}
	else {	
	n=0;
	}
	
	
}

	
 __irq void TIMER1_IRQHandler (void) 
{  
	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
	Lauflicht();
}