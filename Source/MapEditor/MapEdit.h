#ifndef MEdit
#define MEdit

#include "LPC17xx.h"
typedef struct Linea line;
typedef struct Block blck;
static unsigned char lindex=0;

struct Linea
{
	int x1;
	int x2;
	int y1;
	int y2;
};

struct Block
{
	line l[4];
	int x1;
	int x2;
	int y1;
	int y2;
};


line NewLinea(int x1,int x2,int y1,int y2);
void NewBlock(int x1,int x2,int y1,int y2);

#endif