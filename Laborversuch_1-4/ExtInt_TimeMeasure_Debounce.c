/******************************************************************************/
/*                                                                            */
/*  ExternInterrupt:  Demo Interrupt / Example: External Interrupt             */
/*                                                                            */
/******************************************************************************/
                  
#include <stdint.h>
#include "LPC17xx.h"
#include "GLCD.h"
#include <stdio.h>

#define RELOAD_VALUE  9999999


int New_INT0_ButtonState, Old_INT0_ButtonState, RisingEdgeDetected, FallingEdgeDetected;
int INT0_PhaseValue, INT0_PulsWidthCounter;
int LowPhaseTime;

 
void Initialize_SysTick(void){
	// Configure SysTick to trigger every millisecond using the CPU Clock
	SysTick->CTRL = 0;                    // Disable the SysTick Module
	/* A clock frequency of 100 MHz is assumed  */
	SysTick->LOAD = RELOAD_VALUE;                // Set the Reload Register for 1Sec interrupts
	NVIC_SetPriority(SysTick_IRQn, 3);    // Set the interrupt priority to least urgency
	SysTick->VAL = 0;                    // Clear the Current Value register
	SysTick->CTRL = (1<<0)|(1<<1)|(1<<2);        // Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
}	
	

void SysTick_Handler(void)
{	
	New_INT0_ButtonState =  ( (1 << 10) & LPC_GPIO2->FIOPIN );
	if (New_INT0_ButtonState){
		 INT0_PhaseValue = 1;
		 FallingEdgeDetected = 0;
 	   if (Old_INT0_ButtonState != New_INT0_ButtonState)
			  RisingEdgeDetected = 1;
		 else
			  RisingEdgeDetected = 0;
	 }
	else {
		 INT0_PhaseValue = 0;
		 RisingEdgeDetected = 0;
		 if (Old_INT0_ButtonState != New_INT0_ButtonState){
			 FallingEdgeDetected = 1;
			 INT0_PulsWidthCounter = 0;
		 }
		 else {
			 FallingEdgeDetected = 0;
			 INT0_PulsWidthCounter++;
		 }
   }
  Old_INT0_ButtonState = New_INT0_ButtonState; 
}	 
			 
			 
    			 	
	
//int main (void) {
//	
//	//char MyStr[20];
//			
//	
//	/* Configure GPIO PORT2.7  .. PORT2.0 as inputs */
//	LPC_GPIO2->FIODIR = 0x00FF;		/* P2.0...P2.7 defined as Outputs */
//  LPC_GPIO2->FIOCLR = 0x00FF;		/* Turn off all LEDs */
//	
//	Initialize_SysTick( );
//	
//	//GLCD_Init();                                    /* Initialize the GLCD */
//  //GLCD_Clear(White);                              /* Clear the GLCD */

//  //GLCD_SetTextColor(Black);                       /* Set the Text Color */
//	//GLCD_DisplayString(1, 0, "  Hallo Gruppe XX");/* print string to LCD */
//	
//	while (1)  {  
//	  if ((RisingEdgeDetected==1) && (INT0_PulsWidthCounter>0)){
//		  
//		RisingEdgeDetected = 0;	
//		LowPhaseTime = INT0_PulsWidthCounter * 100; // in msec as long as RELOAD_VALUE = 9999999
//	  //GLCD_DisplayString(3, 0, "Taster EINT0 wurde");/* print string to LCD */			
//		//sprintf(MyStr, "%d Millisek. lang ", LowPhaseTime);
//		//GLCD_DisplayString(4, 0,MyStr);/* print string to LCD */	
//		//GLCD_DisplayString(5, 0, "lang gedrueck.");/* print string to LCD */				
//	}
// }
//}

