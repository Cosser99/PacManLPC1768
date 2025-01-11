/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"  //mod
#include "../MapEditor/Map.h"
#include "../GameManager/GM.h"
#include <stdio.h>
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;
volatile int direction=0; //0123 sotto destra sopra sinistra
volatile int cango[4]={1};
volatile uint8_t dir=0;
volatile uint8_t pressed=0;

extern Player pac;
extern Player ghost;
extern GINFO Session;
extern uint8_t mode;
//bitmap
extern uint8_t bitmapcircle[8];
extern uint8_t bitmap_superpill[8];
extern uint32_t bitmap_map[NUMY];
extern uint8_t bitmap_ghost[8];
extern uint8_t bitmappills[8];
extern uint16_t bitmap_pac[16];
//
extern int mapmat[MAXCASELLA][NUMY]; //mapmatrix
extern int into_down;
//
static uint8_t frame=0;

void Death()
{
	Session.lives--;
	Session.death=1;
	Session.paused=1;
	
}
int checkposition()
{
	
	int x=pac.x;
	int y=pac.y;
	
	if(mapmat[x][y]==2)	
	{
		Session.score+=10;
		Session.netscore+=10;
		Session.pills=Session.pills-1;
		mapmat[x][y]=1;
	}
	if(mapmat[x][y]==3)	
	{
		Session.score+=50;
		Session.netscore+=50;
		Session.pills=Session.pills-1;
		mapmat[x][y]=1;
		if(mode!=3)
		mode=1;//cambia mod
	}
	//Se prende il fantasmino
	if(x==ghost.x&&y==ghost.y)
	{
		switch(mode)
		{
			case 0:			//IL FANTASMA TI HA PRESO
			if(!Session.death)
			Death();
			
			break;	
			case 1:				//MUORE IL FANTASMA
			Session.score+=100;
			Session.netscore+=100;
			mode=0;	
			ghost.x=30;
			ghost.y=40;
			mode=3;		//MODALITA ANIMAZIONE
			break;
		}
		
	}
	//****************TELETRASPORTO**********
	if(pac.x==0&&dir==4) {LCD_DrawBlock(pac.x*8,pac.y*8,8,Blue);pac.x=30;}
	else if(pac.x==29&&dir==2) {LCD_DrawBlock(pac.x*8,pac.y*8,8,Blue);pac.x=0;}
	//**************************************
	if(mapmat[x][y+1]==0) //sotto
	{
		cango[0]=0;
	}
	if(mapmat[x+1][y]==0)//destra
	{
		cango[1]=0;
	}
	if(mapmat[x][y-1]==0)//sopra
	{
		cango[2]=0;
	}
	if(x!=0)
	if(mapmat[x-1][y]==0)//sinistra
	{
		cango[3]=0;
	}
	
	return 1;
}
void rescango()//reset cango
{
	cango[0]=1;
	cango[1]=1;
	cango[2]=1;
	cango[3]=1;
}
int checkbutton()
{
	//conta quanti 0 ci sono in FIOPIN da 27 a 29
	int value=LPC_GPIO1->FIOPIN;
	int cnt=0,i;
	for(i=0;i<4;i++)
	{
		if((value&(1<<(26+i)))==0) cnt++;
	}
	if(cnt>1)return 0;
	return 1;
}

void UpdateAnim()
{
	static uint8_t nf=0;
	
	if(frame==0)LCD_DrawLine(13*SIZEBLOCK,16*SIZEBLOCK+4,15*SIZEBLOCK,16*SIZEBLOCK+4,Blue);
	if(frame>=4) {
		ghost.x=13;ghost.y=15;mode=0;frame=0;
		LCD_DrawLine(13*SIZEBLOCK,16*SIZEBLOCK+4,15*SIZEBLOCK,16*SIZEBLOCK+4,White);
	return;}
	if(nf)
	{
	LCD_DrawBlock(13*SIZEBLOCK,18*SIZEBLOCK-(frame*8)+8,8,Blue);
	LCD_Drawbitmap(13*SIZEBLOCK,18*SIZEBLOCK-frame*8,Red,8,bitmap_ghost);
	//SetLastPixel(13*SIZEBLOCK,18*SIZEBLOCK-frame*3,lastpixel);
	frame++;
	}
	nf=!nf;
}
void RIT_IRQHandler (void)
{					
	//
	//UpdateText();
	//Joystick : 26 sotto , 27 sinistra , 28 destra , 29 sopra
	int moving=0;	//evita il lampeggio di pacman
	static int up=0;
	static int position=0;	
	
	int lx=pac.x;//lastpos
	int ly=pac.y; 
	if(!Session.gameover&&!Session.paused)
	{
		checkposition();

		if(checkbutton())
		{
			if((((LPC_GPIO1->FIOPIN & (1<<28)) == 0)||dir==2)&&cango[1]){moving=1;dir=2;}	//destra
			if((((LPC_GPIO1->FIOPIN & (1<<26)) == 0)||dir==1)&&cango[0]){moving=1;dir=1;}	//sotto
			if((((LPC_GPIO1->FIOPIN & (1<<29)) == 0)||dir==3)&&cango[2]){moving=1;dir=3;} //up
			if((((LPC_GPIO1->FIOPIN & (1<<27)) == 0)||dir==4)&&cango[3]){moving=1;dir=4;}	//sinistra
		}
		if(moving)	//è brutto vedere il codice in questo modo ma serve per risolvere un po' di problemi
		{
			switch(dir)
			{
				case 1: pac.y++;break;
				case 2: pac.x++;break;
				case 3: pac.y--;break;
				case 4: pac.x--;break;
			}
		}
		rescango();
		if(moving)
		{
			DrawPac( pac.x, pac.y,bitmapcircle);
			LCD_DrawBlock(lx*8,ly*8,8,Blue);
		}
		//************GHOST MOVEMENT******************
		static nextframe=0;
		if(nextframe==0)	//muovere il fantasimo la meta del tempo
		{
			ghost.lx=ghost.x;
			ghost.ly=ghost.y;
		uint8_t movement=Next(&pac,&ghost);
			switch(movement)
			{
				case 0:ghost.y++;  break;
				case 1:ghost.x++;break;
				case 2:ghost.y--;break;
				case 3:ghost.x--;break;
				default: break;
			}
			//Draw
			switch(mapmat[ghost.lx][ghost.ly])
			{
				case 1:LCD_DrawBlock(ghost.lx*SIZEBLOCK,ghost.ly*SIZEBLOCK,8,Blue);	break;
				case 2:LCD_DrawBlock(ghost.lx*SIZEBLOCK,ghost.ly*SIZEBLOCK,8,Blue);
					LCD_Drawbitmap(ghost.lx*SIZEBLOCK,ghost.ly*SIZEBLOCK,Red,8,bitmappills);
								
				break;
				case 3:
						LCD_DrawBlock(ghost.lx*SIZEBLOCK,ghost.ly*SIZEBLOCK,8,Blue);
						LCD_Drawbitmap(ghost.lx*SIZEBLOCK,ghost.ly*SIZEBLOCK,Red,8,bitmap_superpill);	
					
				break;
				default: break;
			}
			switch(mode)
			{
				case 0:LCD_Drawbitmap(ghost.x*SIZEBLOCK,ghost.y*SIZEBLOCK,Red,8,bitmap_ghost); break;
				case 1:LCD_Drawbitmap(ghost.x*SIZEBLOCK,ghost.y*SIZEBLOCK,Blue2,8,bitmap_ghost); break;
				case 3:UpdateAnim();break;
			}
			
			//LCD_DrawBlock(ghost.lx*SIZEBLOCK,ghost.ly*SIZEBLOCK,8,Blue);	

		}
		nextframe=!nextframe;
	}
	//****************VICTORY***********************
	if(Session.pills<=0&&!Session.gameover)
	{
		Session.gameover=1;
		uint8_t txt[16]={"VICTORY!"};
		LCD_Clear(Black);
		GUI_Text(MAX_X/2-20,MAX_Y/2-10,txt,White,Black);
	}
	//***********************************************
	
	//LCD_Drawbitmap(pac.x/8*8,pac.y/8*8,Blue,8,bitmapcircle);
	//LCD_Drawbitmap(pac.x,pac.y,Yellow,8,bitmapcircle);
	//LCD_Drawbitmap(pac.x,pac.y,Blue,8,bitmapcircle);
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		//up++;
		//LCD_SetPoint(120,position,Black);
		//position++;
		switch(up){
			case 1:
				
				//LED_Off(position);
				//LED_On(0);
				//LCD_SetPoint(120,position,Black);
				//position++;
				break;
			case 60:	//3sec = 3000ms/50ms = 60
				//LED_Off(position);
				//LED_On(7);
				//position = 7;
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	
	/* button management */
	if(into_down!=0){ 
		into_down++;
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INTO pressed */
			switch(into_down){				
				case 2:				/* pay attention here: please see slides to understand value 2 */
					Session.paused=!Session.paused;
					Pause(&Session,MAX_X/2-30,MAX_Y/2-20);
				break;
				default:
					break;
			}
			
		}
		else {	/* button released */
			into_down=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
/*	else{
			if(down==1)
				down++;
	} */
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
