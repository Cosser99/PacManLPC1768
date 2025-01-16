
/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "MapEditor/MAP.h"
#include "GameManager/GM.h"
#include "CAN/CAN.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
//
#define NUMPILLS 240

extern Player pac;
extern Player ghost;
extern uint16_t bitmap_pac[16];
extern uint8_t bitmapcircle[8];
extern uint8_t bitmap_superpill[8];
extern uint32_t bitmap_map[NUMY];
extern uint32_t spawnpill[NUMY];
extern uint8_t bitmappills[8];
extern int mapmat[MAXCASELLA][NUMY]; //mapmatrix
extern uint32_t LASTBIT;
GINFO Session;
//
int into_down;
uint8_t mode=0; //modalità del fantasma in corso

static int cnt1=0;	//DEBUG caselle a 1

#define TRACECOLOR Blue
void ConfigMap()
{
	int i,j;
	for(i=0;i<NUMY;i++)
	{
		uint32_t riga=bitmap_map[i];
		for(j=0;j<MAXCASELLA+1;j++)
		{
			if(riga&1)
			{
				//disegna quadrato 16x16
				LCD_DrawBlock(j*SIZEBLOCK,i*SIZEBLOCK,SIZEBLOCK,TRACECOLOR); //j sarebbe x sul display , i y 
				mapmat[j][i]=1;
				cnt1++;
			}
			riga=riga>>1;
		}
	}
	//tana fantasma e porta		
	// x10 y17
	for(i=10;i<18;i++)
	{
		for(j=17;j<20;j++)
		{
			LCD_DrawBlock(i*SIZEBLOCK,j*SIZEBLOCK,SIZEBLOCK,TRACECOLOR);
		}
	}
	LCD_DrawBlock(13*SIZEBLOCK,16*SIZEBLOCK,SIZEBLOCK,TRACECOLOR);
	LCD_DrawBlock(14*SIZEBLOCK,16*SIZEBLOCK,SIZEBLOCK,TRACECOLOR);
	LCD_DrawLine(13*SIZEBLOCK,16*SIZEBLOCK+4,15*SIZEBLOCK,16*SIZEBLOCK+4,White);
}

void DrawPills()
{
	int i,j;
	int num=NUMPILLS;
	for(i=0;i<NUMY&&num>0;i++)
	{
		uint32_t riga=spawnpill[i];
		for(j=0;j<MAXCASELLA&&num>0+1;j++)
		{
			if(riga&1)
			{
				//LCD_SetPoint(i*SIZEBLOCK,j*SIZEBLOCK,Yellow); pixel di riferimento
				LCD_Drawbitmap(j*SIZEBLOCK,i*SIZEBLOCK,Red,8,bitmappills);
				mapmat[j][i]=2;
				Session.pills++;
				num--;
			}
			riga=riga>>1;
		}
	}
}
void DebugOffset(uint16_t size,uint16_t color)	//DebugOffset(SIZEBLOCK,Black);
{
	int i;
	for(i=0;i<MAXCASELLA;i++)
	{
		LCD_SetPoint(i*size,0,color);
	}
}
void SetPrio()
{
	NVIC_SetPriority(RIT_IRQn,1);
	NVIC_SetPriority(TIMER0_IRQn,0);
	NVIC_SetPriority(TIMER1_IRQn,0);
	NVIC_SetPriority(TIMER3_IRQn,0);
	#ifndef SIMULATOR
	NVIC_SetPriority(CAN_IRQn,4);
	#endif
}
int GenRandom(int max)
{
	srand(LPC_TIM0->TC^LPC_RIT->RICOUNTER);
	return rand()%max;
}


int main(void) 
{
	
	//
		GM_Default(&Session);
	//
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();
  LCD_Initialization();
	#ifndef SIMULATOR
	CAN_Init();
	#endif
	
	LCD_Clear(Black);
	GUI_Text(0,0,(uint8_t*) "Game Over In:",White,Black);
  GUI_Text(150,0,(uint8_t*) "SCORE:",White,Black);
	pac.x=Session.spx;
	pac.y=Session.spy;
	ConfigMap();

	DrawPills();
	ghost.x=13;
	ghost.y=15;
	ghost.mode=0;
	LCD_Drawbitmap(pac.x*SIZEBLOCK,pac.y*SIZEBLOCK,Yellow,8,bitmapcircle);
	init_timer(0, 0x017D7840 ); 						    // contatore tempo
	joystick_init();											//Joystick Initialization            
	
	#ifdef SIMULATOR
	init_RIT(0x002C4B40);									// Emulatore 0x002C4B40	(Impostare lo SPEEDUP 10 TIMER0 1 2 3 : 1 , RIT scaler : 1)
	#else
	init_RIT(0x007C4B40);									// Scheda 0x007C4B40
	#endif
	//init_RIT(0x003C4B40); //0x004C4B40
	enable_RIT();													// RIT enabled												
	enable_timer(0);
	SetPrio();  //altrimenti non funziona bene il rit
	
	static uint8_t a[7]={"Lives:"};
	GUI_Text(0,300,a,White,Black);
	LCD_Drawbitmap16(50,300,Yellow,16,bitmap_pac);
	//Metto in pausa in modo che possa prendere un tempo casuale per lo spawn delle superpill
	GUI_Text(MAX_X/2-25,MAX_Y/2-20,(uint8_t*)"INTO",White,Black);
	Session.paused=1;
	init_timer(2,0x00FFFFFF);
	enable_timer(2);
	
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	//importante (per la musica) non togliere
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	//
  while (1)	
  {
		__ASM("wfi");
  }
}

