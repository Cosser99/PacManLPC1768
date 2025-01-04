/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

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
				LCD_DrawBlock2(j*SIZEBLOCK,i*SIZEBLOCK,SIZEBLOCK,TRACECOLOR); //j sarebbe x sul display , i y 
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
			LCD_DrawBlock2(i*SIZEBLOCK,j*SIZEBLOCK,SIZEBLOCK,TRACECOLOR);
		}
	}
	LCD_DrawBlock2(13*SIZEBLOCK,16*SIZEBLOCK,SIZEBLOCK,TRACECOLOR);
	LCD_DrawBlock2(14*SIZEBLOCK,16*SIZEBLOCK,SIZEBLOCK,TRACECOLOR);
	LCD_DrawLine(13*SIZEBLOCK,16*SIZEBLOCK+4,15*SIZEBLOCK,16*SIZEBLOCK+4,White);
}
void DrawPills()
{
	int i;
	int num=NUMPILLS;
	for(i=0;i<MAXCASELLA&&num>0;i++)
	{
		
		int j;
		for(j=0;j<NUMY&&num>0;j++)
		{
			if(mapmat[i][j]==1)
			{
				//LCD_SetPoint(i*SIZEBLOCK,j*SIZEBLOCK,Yellow); pixel di riferimento
				LCD_DrawCircle(i*SIZEBLOCK,j*SIZEBLOCK,Red,8,bitmappills);
				mapmat[i][j]=2;
				Session.pills++;
				num--;
			}
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
	NVIC_SetPriority(RIT_IRQn,0);
	NVIC_SetPriority(TIMER0_IRQn,3);
	
}
int GenRandom(int max)
{
	srand(LPC_TIM0->TC^LPC_RIT->RICOUNTER);
	return rand()%max;
}

void GenSuperPill()
{
	//x 0-30 y .. 0-40
	int counter=NSUPERPILL;
	do
	{
		int x,y;
		x=GenRandom(MAX_X/8);
		y=GenRandom(MAX_Y/8);
		if(mapmat[x][y]==2)
		{
			mapmat[x][y]=3;
			LCD_DrawCircle(x*SIZEBLOCK,y*SIZEBLOCK,Red,8,bitmap_superpill);
			counter--;
		}
	}while(counter !=0);
}
int main(void) 
{
	//
		GM_Default(&Session);
	//
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();
  LCD_Initialization();
		//TP_Init();
		//TouchPanel_Calibrate();

//				LCD_SetPoint(16,10,Green);		//X offseet +6	Y normale , GetPoint(10,10,Green)
//				LCD_SetPoint(20,1,Black);
//				if(LCD_GetPoint(20-6,1)==Black) LCD_DrawBlock2(0,100,20,Black);


	LCD_Clear(Black);
	GUI_Text(0,0,(uint8_t*) "Game Over In:",White,Black);
		//GUI_Text(150,0,(uint8_t*) "SCORE:",White,Black);
	pac.x=Session.spx;
	pac.y=Session.spy;
	ConfigMap();
	DrawPills();
	ghost.x=18;
	ghost.y=29;
	ghost.mode=0;
	LCD_DrawCircle(pac.x*SIZEBLOCK,pac.y*SIZEBLOCK,Yellow,8,bitmapcircle);
		//init_timer(0, 0x1312D0 ); 						// 50ms * 25MHz = 1.25*10^6 = 0x1312D0 
		//init_timer(0, 0x6108 ); 						  // 1ms * 25MHz = 25*10^3 = 0x6108 
		//init_timer(0, 0x4E2 ); 						    // 500us * 25MHz = 1.25*10^3 = 0x4E2 
	//17D 7840
	init_timer(0, 0x017D7840 ); 						    // 8us * 25MHz = 200 ~= 0xC8 
	joystick_init();											//Joystick Initialization            
	init_RIT(0x002C4B40);									// RIT Initialization 50 msec   
	init_timer(1,0x002C4B40);							//Timer for score
	init_timer(2,0x009FFFFF);
	//init_RIT(0x003C4B40); //0x004C4B40
	enable_RIT();													// RIT enabled												
	enable_timer(0);
	enable_timer(1);
	enable_timer(2);
	
	//GenSuperPill();
	SetPrio();  //altrimenti non funziona bene il rit
	LPC_SC->PCON |= 0x1;									// power-down	mode										
	LPC_SC->PCON &= ~(0x2);						
		
	static uint8_t a[7]={"Lives:"};
	GUI_Text(0,300,a,White,Black);
	LCD_DrawCircle16(50,300,Yellow,16,bitmap_pac);
	
//	LCD_DrawCircle16(10,10,Yellow,16,bitmap_pac);
//	LCD_DrawCircle16(18,100,Yellow,16,bitmap_pac);
//	LCD_DrawCircle16(26,100,Yellow,16,bitmap_pac);
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
