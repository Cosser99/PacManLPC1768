#include "music.h"

#include "../timer/timer.h"

#define UPTICKS 1
NOTE song[] = 
{
	//1
	 {C3,time_semicroma},
	 {C4,time_semicroma},
	 {G3,time_semicroma}, 
	 {E3,time_semicroma},
	 {C4,time_semicroma},
	 {G3,time_semicroma},
	 {E3,time_croma},
	 //2
	 {CS3,time_semicroma},
	 {CS4,time_semicroma},
	 {GS3,time_semicroma}, 
	 {E3,time_semicroma},
	 {CS4,time_semicroma},
	 {GS3,time_semicroma},
	 {E3,time_croma},
	 	 //3
	 {C3,time_semicroma},
	 {C4,time_semicroma},
	 {G3,time_semicroma}, 
	 {E3,time_semicroma},
	 {C4,time_semicroma},
	 {G3,time_semicroma},
	 {E3,time_croma},
	 //4
	 {FS3,time_biscroma},//sol diesis
	 {F3,time_biscroma},
	 {GS3,time_biscroma},
	 {GS3,time_biscroma},//la diesis
	 {A3,time_biscroma},
	 {AS3,time_biscroma},
	 {AS3,time_biscroma},//si diesis
	 {A3,time_biscroma},
	 {C4,time_semicroma},
	 {C4,time_semicroma}

};
NOTE ingame[]=
{
	//da fare in ripetizione
	{C2,time_biscroma},
	{D2,time_biscroma},
	{C2,time_biscroma},
	{D2,time_biscroma},
		{pause,time_biscroma}
};
NOTE death[]=
{
	//mi fa re# re do -1 sol# fa# -1 do# do#
	{E3,time_biscroma},
	{F3,time_biscroma},
	{DS3,time_biscroma},
	{D3,time_biscroma},
	{C3,time_biscroma},
	{GS2,time_biscroma},
	{FS2,time_biscroma},
	{pause,time_biscroma},
	{pause,time_biscroma},
	{CS1,time_biscroma},
	{C1,time_semicroma},
	{pause,time_biscroma}
};


void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(3);
		//reset_timer(2);
		init_timer(3, note.freq);
		//init_timer(2,note.freq);
		enable_timer(3);
		//enable_timer(2);
	}
	reset_timer(1);
	init_timer(1, note.duration);
	enable_timer(1);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM3->TCR != 0) || (LPC_TIM1->TCR != 0));
}
