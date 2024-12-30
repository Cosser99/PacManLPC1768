#include "Map.h"


//Mappa
// X : 240 Y: 320



int mapmat[MAXCASELLA][NUMY]={0}; //mapmatrix


uint32_t LASTBIT=2147483648;
Player pac;
//Questo in bit rappresentano dove disegnare il punto
//Setter Getter
uint8_t GetX(Player x){return x.x;}
uint8_t GetY(Player x){return x.y;}
uint8_t GetScore(Player x){return x.score;}
//setter
void SetX(Player x,uint8_t value){x.x=value;}
void SetY(Player x,uint8_t value){x.y=value;}
void SetScore(Player x,uint8_t value){x.score=value;}

uint8_t bitmappills[8]=
{
	0,
	0,
	0,//126
	24,
	0,
	0,
	0,
	0
	
};
uint8_t bitmapcircle[8]=
{
	0,
	0,
	24,//126
	60,
	60,
	24,
	0,
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

