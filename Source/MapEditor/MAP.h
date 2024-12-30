#ifndef MAPINC
#define MAPINC

#include "LPC17xx.h"
//Mappa
// X : 240 Y: 320

#define MAXCASELLA 30
#define SIZEBLOCK 8
//#define LASTBIT 2147483648	
#define NUMY 40
#define NSUPERPILL 6


typedef struct PLAYER Player;
struct PLAYER
{
	uint8_t x;
	uint8_t y;
	uint16_t score;
};
//Setter Getter
uint8_t GetX(Player x);
uint8_t GetY(Player x);
uint8_t GetScore(Player x);
//setter
void SetX(Player x,uint8_t value);
void SetY(Player x,uint8_t value);
void SetScore(Player x,uint8_t value);



//


#endif