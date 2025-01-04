//GameManagerInfo
#ifndef GAMEMANAGER
#define GAMEMANAGER

#include "LPC17xx.h"
#include "../GLCD/GLCD.h"
typedef struct INFO GINFO;
struct INFO
{
	uint8_t lives;
	uint16_t score;
	uint16_t netscore;
	uint8_t gameover;
	uint8_t paused;
	uint8_t time;
	uint16_t pills;
	uint8_t superpills;
	//spawnpoint
	uint8_t spx;
	uint8_t spy;
	//
	uint8_t death;
	
};
void GM_Default(GINFO *info);
void Pause(GINFO *info,int posx,int posy);
#endif
