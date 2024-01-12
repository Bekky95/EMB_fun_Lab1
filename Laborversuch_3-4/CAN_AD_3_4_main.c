
/*----------------------------------------------------------------------------
 * Name:    CAN_AD_3_4_main.c
 * Purpose: main File for LPC1768 Microcontrollers        
 * Version: Version 1.0 / 02.05.2019 (KBL, HS-Mannheim)
 *----------------------------------------------------------------------------*/


#include "LPC17xx.h"
#include "adc.h"
#include "GLCD.h"
#include <stdio.h>

#include <stdlib.h>

extern void  ADC_StartConversion(void);

void Timer1_Config(void);


const uint16_t My_CAN_ID = 0x701;

uint8_t  AD_New_Value_Ready;
float AD_Voltage;
char NewLine[21];
volatile char AD_Val_String[6];


int main(void)
{
	GLCD_Init();                                    /* Initialize the GLCD */
	GLCD_Clear(White);                              /* Clear the GLCD */
  GLCD_SetBackColor(Blue);                        /* Set the Back Color */
  GLCD_SetTextColor(White);                       /* Set the Text Color */
	
	GLCD_DisplayString(0, 0, "  EMB Lab3: CAN/ADC ");/* print string to LCD */
	sprintf(NewLine, "   Local ID: %#3x  ", My_CAN_ID);
	GLCD_DisplayString(1, 0, (unsigned char *) NewLine);/* print string to LCD */
	GLCD_DisplayString(2, 0, "Local ADValue:");	
	GLCD_DisplayString(2, 19, "V");	
	
	Timer1_Config();
	ADC_Config();
  ADC_StartConversion();
	
	AD_New_Value_Ready = 0;
	
	while (1) 
		{
			int i;
			if(AD_New_Value_Ready)
				{
					GLCD_SetBackColor(Blue);                        /* Set the Back Color */
					GLCD_SetTextColor(White);                       /* Set the Text Color */

					GLCD_DisplayString(2, 14, (unsigned char*)AD_Val_String);
										
					//GLCD_DisplayString(2, 14, "      ");						// Wert leeren:
					
					AD_New_Value_Ready = 0;													// Reset new value
				}
		}
}

void Timer1_Config(void)
{
	//Put some code here
	
	NVIC_EnableIRQ(TIMER1_IRQn); 				// Put some code in order to enable Timer1 interrupt 
	NVIC_SetPriority(TIMER1_IRQn, 1);		// with priority level 1
	
	LPC_SC->PCONP |= (1<<2);						// enable Timer1
	
	LPC_TIM1->PR = (25000 - 1);					// let Timer1 generate an Interrupt every 100 ms
	LPC_TIM1->CTCR = 0x0;								// Timer Modus - TC++ bei PC = PR, PC++ bei rising PCLK edge
	LPC_TIM1->MCR = (3 << 0);					  // falling & rising edge Interrupt
	LPC_TIM1->MR0 = (10);					  		// 10msek
	
	LPC_TIM1->TCR = (1 << 1);						// Timer 1 reset
	LPC_TIM1->TCR = (1 << 0);						// Timer 1 start
}


void TIMER1_IRQHandler (void) 
{  
	uint16_t AD_Value;
	float Max_Voltage = 3.3;
	float Quantline = 4095.0;		// 2^12-1
	
	//Put some code here
	
	//AD_Value = LPC_ADC->ADDR5;								// Read and store AD value 
	//AD_Value = (LPC_ADC->ADDR5 & 0xFFF0);			// DIE NULL LÖSCHT DIE ERSTEN 4 BITS -> BÖSE !!!!! 
	AD_Value = ((LPC_ADC->ADDR5>>4) & 0xFFF);		// Bit 4-15 
	//AD_Value = ((LPC_ADC->ADGDR >> 4) & 0xFFF);
	
	// AD_Value in Voltage berechnen und zu string convertieren:
	AD_Voltage = (AD_Value * Max_Voltage) / Quantline;
	sprintf(AD_Val_String, "%.3f", AD_Voltage);
	
	ADC_StartConversion();											// Start new AD conversion
	
	AD_New_Value_Ready = 1;											// set new value
	
	LPC_TIM1->IR |= (1 << 0);										// Clear timer1 interrupt
}


/************************************ EOF ***********************************/


