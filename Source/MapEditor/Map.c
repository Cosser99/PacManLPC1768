#include "Map.h"
#include <math.h>
#include <stdlib.h>
//Mappa
// X : 240 Y: 320



int mapmat[MAXCASELLA][NUMY]={0}; //mapmatrix
//
Player pac;
Player ghost;
//Questo in bit rappresentano dove disegnare il punto
//Setter Getter
uint8_t GetX(Player x){return x.x;}
uint8_t GetY(Player x){return x.y;}
//setter
void SetX(Player x,uint8_t value){x.x=value;}
void SetY(Player x,uint8_t value){x.y=value;}

uint32_t spawnpill[]={
0 ,
0 ,
0 ,
0 ,
0 ,
536870910 ,
268435458 ,
285343746 ,
514359306 ,
335560722 ,
391774210 ,
285300050 ,
369033218 ,
268502018 ,
536412502 ,
269483778 ,
268960002 ,
536347134 ,
268960002 ,
268960002 ,
268960002 ,
536870910 ,
285343874 ,
285343874 ,
289822170 ,
285868162 ,
289538178 ,
536870910 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 };

uint8_t bitmappills[8]=
{
	0,
	0,
	24,//126
	24,
	0,
	0,
	0,
	0
	
};
uint8_t bitmap_ghost[8]=
{
24,
60,
	126,
	90,
	126,
	126,
	90,
	0
};
uint8_t bitmapcircle[8]=
{
	0,
	24,
	60,//24
	126,//60
	126,
	60,
	24,
	0
	
};
uint8_t bitmap_superpill[8]=
{
	0,
	24,
	60,//126
	126,
	126,
	60,
	24,
	0
	
};
uint16_t bitmap_pac[16]		//pacman lives
=
{
	2016,
	8184,
	16380,
	16318,
	16382,
	8190,
	2047,
	511,
	255,
	511,
	2047,
	8190,
	16382,
	16382,
	8184,
	2016	
};
/*
uint32_t bitmap_map[NUMY]={
0 ,
0 ,
0 ,
0 ,
0 ,
536870910 ,
287449634 ,
287449634 ,
536870910 ,
335560706 ,
402604034 ,
360808446 ,
368918530 ,
335626242 ,
536870910 ,
269483778 ,
268960002 ,
1073218047 ,
1073218047 ,
268960018 ,
268960018 ,
536870910 ,
268443730 ,
268443730 ,
536870910 ,
300121746 ,
300089490 ,
300122110 ,
505348242 ,
505380754 ,
403177470 ,
403111954 ,
434601970 ,
536838142 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 };
*/
uint32_t bitmap_map[]={//23
0 ,
0 ,
0 ,
0 ,
0 ,
536870910 ,
285343746 ,
285343746 ,
536854558 ,
336085010 ,
402604050 ,
352419826 ,//
369034242 ,
335610882 ,
536413694 ,
269483778 ,
268960002 ,
1073218047 ,
268960002 ,
268960002 ,
268960002 ,
536870910 ,
285343874 ,
285343874 ,
298745854 ,
290062466 ,
290062466 ,
536870910 ,
0 ,
0 };
//

//Funzioni
uint16_t Distance(int x1,int y1,int x2,int y2)
{

	int a,b;
	if(x2<x1) a=x1-x2;
	else a=x2-x1;
	
	if(y2<y1) b=y1-y2;
		else b=y2-y1;
		//Euclidea
	return sqrt(a*a+b*b);
	//Manatthan
	//return abs(x1-x2)+abs(y1-y2);
}
//Variabili
/*
PriorityQueue openSet;
uint8_t comeFrom[30];
uint8_t dist[MAXCASELLA][NUMY]={2000}; //matrice distanze
*/
//



extern uint8_t mode;
uint8_t Next(Player *pacman,Player *blinky) //return dir 0sotto 1 destra 2 sopra 3 sinistra
{
	 uint8_t lastpx=blinky->lx;
	 uint8_t lastpy=blinky->ly;
	uint8_t tx=pacman->x;	//target position (i ghost nel gioco originale giravano in loop per delle zone)
	uint8_t ty=pacman->y;
	uint8_t distv[4]={100,100,100,100}; //Distance vector
	uint8_t cango[4]={0,0,0,0};
	int x,y;
	x=blinky->x;
	y=blinky->y;

	uint8_t mindir=100;
	uint8_t maxdir=0;
	int index=-1;
	switch(mode){
	case 0: //if(Distance(x,y,pac.x,pac.y)<10){tx=pacman->x;ty=pacman->y;}
	if(mapmat[x][y+1]!=0&&!(lastpx==x&&lastpy==(y+1))){distv[0]=Distance(x,y+1,tx,ty);}//sotto
	if(mapmat[x+1][y]!=0&&!(lastpx==(x+1)&&lastpy==y)){distv[1]=Distance(x+1,y,tx,ty);}//destra
	if(mapmat[x][y-1]!=0&&!(lastpx==x&&lastpy==(y-1))){distv[2]=Distance(x,y-1,tx,ty);}//sopra
	if(mapmat[x-1][y]!=0&&!(lastpx==(x-1)&&lastpy==y)){distv[3]=Distance(x-1,y,tx,ty);}//sinistra
	if(x-1==0)distv[3]=0xFF;
	if(x+1==29)distv[1]=0xFF;
	int i;
	for(i=0;i<4;i++) {
	if(distv[i]<mindir)
		{
			mindir=distv[i];
			index=i;
		}
	}
			break;
		case 1: 
	if(mapmat[x][y+1]!=0&&!(lastpx==x&&lastpy==(y+1))){distv[0]=Distance(x,y+1,tx,ty);cango[0]=1;}//sotto
	if(mapmat[x+1][y]!=0&&!(lastpx==(x+1)&&lastpy==y)){distv[1]=Distance(x+1,y,tx,ty);cango[1]=1;}//destra
	if(mapmat[x][y-1]!=0&&!(lastpx==x&&lastpy==(y-1))){distv[2]=Distance(x,y-1,tx,ty);cango[2]=1;}//sopra
	if(mapmat[x-1][y]!=0&&!(lastpx==(x-1)&&lastpy==y)){distv[3]=Distance(x-1,y,tx,ty);cango[3]=1;}//sinistra
	if(x-1==0)distv[3]=0;
	if(x+1==29)distv[1]=0;
	for(i=0;i<4;i++) {
	if(distv[i]>maxdir&&cango[i]==1)
		{
			maxdir=distv[i];
			index=i;
		}
	}

			
		break;
}
	
	return index;
}
