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
	uint8_t lx;  //last position
	uint8_t ly; 
};
//Setter Getter
uint8_t GetX(Player x);
uint8_t GetY(Player x);
//setter
void SetX(Player x,uint8_t value);
void SetY(Player x,uint8_t value);

void Path(Player *pacman,Player *blinky);
uint8_t Next(Player *pacman,Player *blinky); //return dir 0sotto 1 destra 2 sopra 3 sinistra

//


#endif