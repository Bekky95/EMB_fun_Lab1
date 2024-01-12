
#include "lpc17xx.h"                              /* LPC17xx definitions    */
#include "adc.h"

/*----------------------------------------------------------------------------
  config ADC
 *----------------------------------------------------------------------------*/
void ADC_Config (void) {

  //Put your code here to perform the following tasks:
  //        1. Enable the AD Converter in the PCONP Register
	// 							Remark: On reset, the ADC is disabled. To enable the ADC, first set the PCADC bit,
	// 							and then enable the ADC in the AD0CR register (bit PDN Table 532). To disable the
	// 							ADC, first clear the PDN bit, and then clear the PCADC bit.
  //				2. Configure the AD Pin  -->  AD0.5 Pin (GPIO 1.31)
  //				3. Select AD channel, AD Frequency, enable AD function in the AD module 
	
	// 1.
	LPC_SC->PCONP |= (1 << 12);					// PCAD-Bit = Bit 12 in PCONP - A/D converter (ADC) power/clock control bit 
																			// !!! Clear PDN-Bit in AD0CR bevore clearing this Bit and 
																			// set this Bit before setting PDN !!!
																			
																			// (1 << 21): PDN-Bit = Bit 21 in ADCR - ADC operational set: 
																			// (4 << 8): 0000 0100 0000 0000 -> PCLK_ADC0/(4+1) 
																			// (1 << 5): 0010 0000 -> Pin 5 für ADC0.5
	LPC_ADC->ADCR = (1 << 21) | (4 << 8) | (1 << 5);  
	
	//LPC_SC->PCLKSEL0 |= (3 << 24); 			// Peripheral Clock Selection register0 
																			// ->  Peripheral clock selection for ADC Bit 25:24 = 11
	// 2.
	LPC_PINCON->PINSEL3 |= (3 << 30);		// AD05 Pin = PINSEL3 Bit 31:30 set 11 - AD-Pin
	
	
//	NVIC_EnableIRQ(ADC_IRQn);						// Interrupt enable in NVIC
}

/*----------------------------------------------------------------------------
  start ADC Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartConversion (void) {
  //Put code to start an AD conversion                    /* start conversion */
  //Please remember that a new conversion must start every 100 ms 
	
	LPC_ADC->ADCR |= (1<<24);						// ADC starten
}



/*********************************************************************************
**                            End Of File
*********************************************************************************/
