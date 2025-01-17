
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../GameManager/GM.h"
#include "../MapEditor/MAP.h"
#include <stdio.h> /*for sprintf*/


#define VOLUME 3
extern GINFO Session;
extern uint16_t bitmap_pac[16];
extern uint8_t bitmap_superpill[8];
extern uint8_t bitmapcircle[8];
extern int mapmat[MAXCASELLA][NUMY];
extern uint8_t mode;

volatile uint8_t ghostrtime=3;
volatile uint8_t gameover=0;
volatile uint8_t counterf=10; //10 secondi
volatile uint8_t spawnyn=1; //aspetta 1 secondo prima di spawnare l'altra pillola
uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};



int GenRandom2(int max)
{
	srand(LPC_TIM1->TC^LPC_RIT->RICOUNTER^LPC_TIM2->TC);
	return rand()%max;
}

void GameOver()
{
		Session.gameover=1;
		LCD_Clear(Black);
		GUI_Text(MAX_X/2-35,MAX_Y/2-20,(uint8_t*)"GAME OVER",White,Black);
		GUI_Text(150,0,(uint8_t*) "SCORE:",White,Black);
}
extern Player pac;
extern Player ghost;

void Respawn()
{
	
	Session.paused=0;
	Session.death=0;
	mode=3;	//Animazione respawn ghost
	LCD_DrawBlock(pac.x*SIZEBLOCK,pac.y*SIZEBLOCK,8,Blue);
	pac.x=Session.spx;
	pac.y=Session.spy;

	LCD_Drawbitmap(pac.x*SIZEBLOCK,pac.y*SIZEBLOCK,Yellow,8,bitmapcircle);
	
	
}
void TIMER0_IRQHandler (void)
{
	if(mode==4)ghostrtime--;		//Aspetta 3 secondi per il respawn del ghost
	if(ghostrtime==0){ghostrtime=3;mode=3;}
	static uint8_t dtimer=0;
	if(dtimer>=2&&Session.death)
	{
		if(Session.lives>0)
			Respawn();
		else
			GameOver();
		dtimer=0;
	}
	else dtimer++;
	if(!Session.gameover&&!Session.paused)
	{
		Session.time--;
		if(Session.time<=0) GameOver();

	}
	if(Session.superpills>0)
	{
		init_timer(2,GenRandom2(0x01FFFFFF));
		LPC_TIM2->TC=0;
		enable_timer(2);
		spawnyn=1;
		
	}
	if(mode) //10 secondi
	{
		counterf--;
	}
	if(counterf<=0){mode=0;counterf=10;}
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
	//UpdateText2();
  return;
}


void TIMER1_IRQHandler (void)
{
	disable_timer(3);
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}
void TIMER2_IRQHandler (void)
{
	//Spawn Super Pill

	if(!Session.gameover&&!Session.paused&&Session.superpills>0)
	{
			int x=GenRandom2(MAXCASELLA);
			int y=GenRandom2(NUMY);
		if(mapmat[x][y]==2&&spawnyn)
			{
				Session.superpills--;
				mapmat[x][y]=3;
				LCD_Drawbitmap(x*SIZEBLOCK,y*SIZEBLOCK,Red,8,bitmap_superpill);
				spawnyn=0;
			}
			//LCD_SetPoint(x*SIZEBLOCK,y*SIZEBLOCK,White); //DEBUG ONLY
	}
	if(Session.superpills<=0) disable_timer(2);
	LPC_TIM2->IR = 1;	
	return;
}
//TAG: TIMER MUSICA ONDA TIMER3 LPC_DAC
void TIMER3_IRQHandler (void)
{
		static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= VOLUME;		//volume
	currentValue += 410;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;

	
	LPC_TIM3->IR = 1;
}	

/******************************************************************************
**                            End Of File
******************************************************************************/
