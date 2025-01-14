#include "GM.h"

void GM_Default(GINFO *info)
{
	info->lives=10;
	info->score=0;
	info->netscore=0;
	info->gameover=0;
	info->spx=3;
	info->spy=5;
	info->time=60;
	info->pills=0; //le conta lui
	info->paused=0;
	info->superpills=6;
	
	info->death=0;
}

void Pause(GINFO *info,int posx,int posy)
{
	int i,j;
	int x,y;
	if(info->paused)
	{
		//Stampo
		uint8_t testo[15]={"PAUSED"};
		GUI_Text(posx,posy,testo,White,Black);
	}
	else
	{
	//reset
		for( i=posy;i<posy+16;i++)
		{
			for( j=posx;j<posx+50;j++)
			{
				LCD_SetPoint(j,i,Blue);
			}
		}
		
	}
	
}