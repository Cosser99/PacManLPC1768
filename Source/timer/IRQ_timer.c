/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../GameManager/GM.h"
#include "../MapEditor/MAP.h"
#include <stdio.h> /*for sprintf*/

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern GINFO Session;
extern uint16_t bitmap_pac[16];
extern uint8_t bitmap_superpill[8];
extern int mapmat[MAXCASELLA][NUMY];
volatile uint8_t gameover=0;
	
void UpdateText2()	//per renderlo più veloce
{
	static uint8_t txt[10];
	sprintf(txt,"Score: %d",Session.score);
	GUI_Text(150,0,txt,White,Black);
	if(Session.netscore>=1000)
	{
		Session.netscore=0;
		Session.lives+=1;
		int i=0;
		for(i=0;i<Session.lives;i++)
			LCD_DrawCircle16(50+i*20,300,Yellow,16,bitmap_pac);
		
	}
}
int GenRandom2(int max)
{
	srand(LPC_TIM1->TC^LPC_RIT->RICOUNTER);
	return rand()%max;
}

void GameOver()
{
		Session.gameover=1;
		GUI_Text(MAX_X/2-30,MAX_Y/2-30,(uint8_t*)"GAME OVER",White,Black);
}
void TIMER0_IRQHandler (void)
{
	if(!Session.gameover&&!Session.paused)
	{
		Session.time--;
		if(Session.time<=0) GameOver();
		uint8_t txt[3];
		sprintf(txt,"%d",Session.time);
		GUI_Text(100,0,txt,White,Black);
		/*
		if(Session.superpills>0)
		{
		init_timer(2,0x00000FFF);
		enable_timer(2);
		}
		*/
		
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
	//UpdateText2();
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER1_IRQHandler (void)
{
	UpdateText2();
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}
void TIMER2_IRQHandler (void)
{
	//Spawn Super Pill
	
	if(!Session.gameover&&!Session.paused&&Session.superpills>0)
	{
			int inserita=0;
			int x=GenRandom2(MAXCASELLA);
			int y=GenRandom2(NUMY);
		if(mapmat[x][y]==2)
			{
				Session.superpills--;
				mapmat[x][y]=3;
				LCD_DrawCircle(x*SIZEBLOCK,y*SIZEBLOCK,Red,8,bitmap_superpill);
				inserita=1;
			}
	}
	if(Session.superpills<=0) disable_timer(2);
	LPC_TIM2->IR = 1;	
	return;
}
	

/******************************************************************************
**                            End Of File
******************************************************************************/
