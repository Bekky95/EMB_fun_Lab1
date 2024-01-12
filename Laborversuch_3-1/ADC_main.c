
#include "LPC17xx.h"
#include "adc.h"

void Timer1_Config(void);
uint16_t AD_value;


int main(void)
{
	//SystemInit();
	Timer1_Config();
	ADC_Config();
	ADC_StartConversion();										// Start new AD conversion
     while (1); 
}

void Timer1_Config(void)
{
	NVIC_EnableIRQ(TIMER1_IRQn); 				// Put some code in order to enable Timer1 interrupt 
	NVIC_SetPriority(TIMER1_IRQn, 1);		// with priority level 1
	
	LPC_SC->PCONP |= (1<<2);						// enable Timer1
	
	LPC_TIM1->PR = (25 - 1);						// let Timer1 generate an Interrupt every 100 ms
	LPC_TIM1->CTCR = 0x0;								// Timer Modus - TC++ bei PC = PR, PC++ bei rising PCLK edge
	LPC_TIM1->MCR = (3 << 0);					  // falling & rising edge Interrupt
	LPC_TIM1->MR0 = (10000-1);					  // 10msek
	
	LPC_TIM1->TCR = (1 << 1);						// Timer 1 reset
	LPC_TIM1->TCR = (1 << 0);						// Timer 1 start
}

void TIMER1_IRQHandler (void) 
{  
 // Put some code lines here
	//AD_value = LPC_ADC->ADDR5;							// Read and store AD value 
	//AD_value = (LPC_ADC->ADDR5 & 0xFFF0);		//DIE NULL LÖSCHT DIE ERSTEN 4 BITS -> BÖSE !!!!! 
	AD_value = ((LPC_ADC->ADDR5>>4) & 0xFFF);			//Bit 4-15 
	//AD_value = ((LPC_ADC->ADGDR >> 4) & 0xFFF);
	ADC_StartConversion();										// Start new AD conversion
	LPC_TIM1->IR |= (1 << 0);									// Clear timer1 interrupt
}

/************************************ EOF ***********************************/

