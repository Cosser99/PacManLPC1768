#include "button.h"
#include "LPC17xx.h"

//Da togliere
#include "../GLCD/GLCD.h"
extern int into_down;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	into_down=1;
	NVIC_DisableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4&=~(1<<20);
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	//down=1;
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


