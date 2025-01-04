//Trash

void GetLastPixel(uint8_t x,uint8_t y,uint16_t **lpixel)
{
	//Ricordo che c'è un offset
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			lpixel[i][j]=LCD_GetPoint(x+j-6,y+i);
		}
	}
}
void SetLastPixel(uint8_t x,uint8_t y,uint16_t **lpixel)
{
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			LCD_SetPoint(x+j,y+i,lpixel[i][j]);
		}
	}
}

//Get
	uint16_t lastpixel[8][8];
	//GetLastPixel(13*SIZEBLOCK,18*SIZEBLOCK-frame*2,lastpixel);
	int x=13*SIZEBLOCK;
	int y=18*SIZEBLOCK-frame*8;
	int i,j;
	for( i=0;i<8;i++)
	{
		for( j=0;j<8;j++)
		{
			lastpixel[i][j]=LCD_GetPoint(x+j,y+i); 
			lastpixel[i][j]=(~lastpixel[i][j])&~0x7E0; //non so xk inverte i colori
		}
	}
	
	//Set
		y=18*SIZEBLOCK-frame*8;
	for( i=0;i<8;i++)
	{
		for( j=0;j<8;j++)
		{
			LCD_SetPoint(x+j,y+i,lastpixel[i][j]);
		}
	}