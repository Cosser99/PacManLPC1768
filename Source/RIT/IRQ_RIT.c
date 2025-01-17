
#include "LPC17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"  //mod
#include "../MapEditor/Map.h"
#include "../GameManager/GM.h"
#include "../music/music.h"
#include "../CAN/CAN.h"
#include <stdio.h>

#define UPTICKS 1
#define GHOSTCOLOR Grey

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

//musica
static int isplaying=1;
extern NOTE song[];
extern NOTE ingame[];
extern NOTE death[];
NOTE *actual=song;
static uint8_t idm; //music id

void Death()
{
	Session.lives--;
	Session.death=1;
	Session.paused=1;
	isplaying=1;
	actual=&death;
	
}
void collision(uint8_t movement)
{
	uint8_t x=pac.x;
	uint8_t y=pac.y;
	uint8_t gx=ghost.x;
	uint8_t gy=ghost.y;
	//check collision next cell  (altrimenti capita che se hanno movimenti opposti e caselle adiacenti si superano a vicenda senza collidere)
	uint8_t coll=0;
	switch(movement)
	{
		case 0:coll=(gy+1==y&&x==gx)?1:0;break; //sotto
		case 1:coll=(gx+1==x&&y==gy)?1:0;break; //destra
		case 2:coll=(gy-1==y&&x==gx)?1:0;break; //sopra
		case 3:coll=(gx-1==x&&y==gy)?1:0;break; //sinistra
	}
		//Se prende il fantasmino
	if(x==ghost.x&&y==ghost.y||coll)
	{
		extern uint8_t counterf;
		switch(mode)
		{
			case 0:			//IL FANTASMA TI HA PRESO
			if(!Session.death)
			Death();
			counterf=10;
			break;	
			case 1:				//MUORE IL FANTASMA
			Session.score+=100;
			Session.netscore+=100;
			mode=4;
			counterf=10;
			break;
		}
		
	}
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
		if(!(mode==3||mode==4))
			mode=1;//cambia mod
		extern uint8_t counterf;
		counterf=10;
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

void UpdateAnim() //TAG: FRAME ANIMAZIONE
{
	static uint8_t nf=0;
	
	if(frame==0)LCD_DrawLine(13*SIZEBLOCK,16*SIZEBLOCK+4,15*SIZEBLOCK,16*SIZEBLOCK+4,Blue);
	if(frame>=4) {
		ghost.x=13;ghost.y=15;mode=0;frame=0; //x 13 y 15
		LCD_DrawLine(13*SIZEBLOCK,16*SIZEBLOCK+4,15*SIZEBLOCK,16*SIZEBLOCK+4,White);
		nf=0;
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
//CAN			
void Transmit()			//RECEIVE ON IRQ_CAN
{
	CAN_TxMsg.data[0] = Session.time;
	CAN_TxMsg.data[1] = Session.lives;
	CAN_TxMsg.data[2] = Session.score>>8;
	CAN_TxMsg.data[3] = Session.score&~0xFFFF0000;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2; // trasmetti da 1 a 2
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg (1, &CAN_TxMsg);               /* transmit message */
}

void Receive(uint8_t lives,uint8_t time,uint16_t score)
{
	 uint8_t txt[15];
	sprintf(txt,"%d",score);
	GUI_Text(150+16*3,0,txt,White,Black);
	if(!Session.gameover)
	{
	if(Session.netscore>=1000)
	{
		Session.netscore=0;
		Session.lives+=1;
		int i=0;
		for(i=0;i<Session.lives;i++)
			LCD_Drawbitmap16(50+i*20,300,Yellow,16,bitmap_pac);
		
	}
	//tempo
		uint8_t txttime[3];
		sprintf(txttime,"%d",time);
		GUI_Text(100,0,txttime,White,Black);	
	//vite
		int i;
		LCD_DrawBlock(50+(lives)*20,300,16,Black);
		for(i=0;i<lives;i++)
			LCD_Drawbitmap16(50+i*20,300,Yellow,16,bitmap_pac);
	}
}


void PlayMusic()
{
	if(isplaying){
		static int currentNote;
		static int ticks = 0;
		if(!isNotePlaying())
		{
			++ticks;
			if(ticks == UPTICKS)
			{
				ticks = 0;
				playNote(actual[currentNote++]);
			}
		}

		if(actual[currentNote].freq==end)
		{
			
			if(idm==2)
			{
				actual=&ingame;
				currentNote=0;
				isplaying=1;
				return ;
			}
			isplaying=0;
		}

	}
}

void RIT_IRQHandler (void)
{		
	Transmit();
	if(!Session.gameover&&!Session.paused){
	PlayMusic();
	idm=2;
	}
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
		//************GHOST MOVEMENT MOVIMENTO******************
		static nextframe=0;
		uint8_t movement;
		movement=Next(&pac,&ghost);
		collision(movement);
		if(nextframe==0)	//muovere il fantasimo la meta del tempo
		{
			if(frame==0) //TODO : Da vedere
			{
				ghost.lx=ghost.x;
				ghost.ly=ghost.y;
				switch(movement)
				{
					case 0:ghost.y++;  break; //sotto
					case 1:ghost.x++;break; //destra
					case 2:ghost.y--;break; //sopra
					case 3:ghost.x--;break; //sinistra
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
			}
			switch(mode)
			{
				case 0:LCD_Drawbitmap(ghost.x*SIZEBLOCK,ghost.y*SIZEBLOCK,Red,8,bitmap_ghost); break; //inseguimento
				case 1:LCD_Drawbitmap(ghost.x*SIZEBLOCK,ghost.y*SIZEBLOCK,GHOSTCOLOR,8,bitmap_ghost); break; //spaventato
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
		GUI_Text(150,0,(uint8_t*) "SCORE:",White,Black);
	}
	//***********************************************

	
	
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
