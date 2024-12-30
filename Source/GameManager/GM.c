#include "GM.h"
uint16_t buffer[16][150];

void GM_Default(GINFO *info)
{
	info->lives=1;
	info->score=0;
	info->netscore=0;
	info->gameover=0;
	info->spx=3;
	info->spy=5;
	info->time=60;
	info->pills=0; //le conta lui
	info->paused=0;
	info->superpills=6;
}

void Pause(GINFO *info,int posx,int posy)
{
	int i,j;
	int x,y;
	if(info->paused)
	{
		
		//Salvo i pixel

		for(i=posy,y=0;i<posy+16;i++,y++)
		{
			for(j=posx,x=0;j<posx+60;j++,x++)
			{
				buffer[y][x]=LCD_GetPoint(j-6,i);
			}
		}
		//Stampo
	uint8_t testo[15]={"PAUSED"};
	GUI_Text(MAX_X/2-10,MAX_Y/2-10,testo,White,Black);
	}
	else
	{
		//reset
		for(i=posy,y=0;i<posy+16;i++,y++)
		{
			for(j=posx,x=0;j<posx+60;j++,x++)
			{
				LCD_SetPoint(j,i,buffer[y][x]);
			}
		}
	
	}
	
}