/*
 * Work licensed under the Gameblabla's Simple License,
 * see COPYRIGHT for more details.
 * If there's no COPYRIGHT files,
 * send an email to gameblabla@openmailbox.org 
*/

#include "pm.h"
#include "char_font.h"
#include "graphics.h"

unsigned short score = 0 , highscore = 0;
unsigned char scored[6];
unsigned char hscored[6];

unsigned char frametime = 0;
unsigned char speed = 7;
unsigned char speedENN1 = 6 , speedENN2 = 6;
unsigned char x3 , y3;
unsigned char gameMODE = 0;
unsigned char lives = 3;
unsigned char timegameover = 0;

unsigned char enn1state = 0 , enn2state = 0;
unsigned char enn1time = 0 , enn2time = 0;

void highscoreConvert();
void scoreConvert();
int rand_a_b(unsigned short a, unsigned short b);

#define inc_rand() 
do 
{
	++random_num;
} while(0);

void next_rand() 
{
	unsigned char n1,n2;
	n1 = (random_num >> 4) & 0xff;
	n1 *= n1;
	n2 = (random_num << 4) & 0xf0;
	n2 += (random_num >> 12) & 0x0f;
	random_num = n1 + n2 * n2 + 0x1234;
}

int rand_a_b(unsigned short a, unsigned short b)
{
    return rand()%(b-a) +a;
}

unsigned int rand(unsigned int limit) 
{
	next_rand();
	return (random_num & 0x7fff) % limit;
}

void drawbackground(void)
{	
	unsigned char x,y;
	
	for(y=0;y<8;y++)
	{
		for(x=0;x<12;x++)
		{
			TILEMAP[y*16+x] = x+y*12;
		}
	}
}


void drawscore(void)
{
	TILEMAP[0*16+0] = scored[0]+7*12;
	TILEMAP[0*16+1] = scored[1]+7*12;
	TILEMAP[0*16+2] = scored[2]+7*12;
	TILEMAP[0*16+3] = scored[3]+7*12;
	TILEMAP[0*16+4] = scored[4]+7*12;
}

void drawlife(void)
{
	switch(lives)
	{
		case 3:
			TILEMAP[0*16+9] = 10+7*12;
			TILEMAP[0*16+10] = 10+7*12;
			TILEMAP[0*16+11] = 10+7*12;
		break;
		case 2:
			TILEMAP[0*16+9] = 10+7*12;
			TILEMAP[0*16+10] = 10+7*12;
			TILEMAP[0*16+11] = 11+7*12;
		break;
		case 1:
			TILEMAP[0*16+9] = 10+7*12;
			TILEMAP[0*16+10] = 11+7*12;
			TILEMAP[0*16+11] = 11+7*12;
		break;
		default:
			TILEMAP[0*16+9] = 11+7*12;
			TILEMAP[0*16+10] = 11+7*12;
			TILEMAP[0*16+11] = 11+7*12;
		break;
	}
	
}

void drawhighscore(void)
{
	TILEMAP[7*16+0] = 11+7*12;
	TILEMAP[7*16+1] = 11+7*12;
	TILEMAP[7*16+2] = 11+7*12;
	TILEMAP[7*16+3] = 11+7*12;
	TILEMAP[7*16+4] = 11+7*12;
	TILEMAP[7*16+5] = 11+7*12;
	TILEMAP[7*16+6] = 11+7*12;
	TILEMAP[7*16+7] = 11+7*12;
	TILEMAP[7*16+8] = 11+7*12;
	TILEMAP[7*16+9] = 11+7*12;

	TILEMAP[7*16+0] = hscored[0]+7*12;
	TILEMAP[7*16+1] = hscored[1]+7*12;
	TILEMAP[7*16+2] = hscored[2]+7*12;
	TILEMAP[7*16+3] = hscored[3]+7*12;
	TILEMAP[7*16+4] = hscored[4]+7*12;
}

void controlrainbowdash(void)
{
	frametime++;
	
	if (frametime > 0)
	{
		OAM[2].tile = OAM[2].tile + 2;
		OAM[3].tile = OAM[3].tile + 2;
		
		if ( OAM[2].tile > 4)
		{
			OAM[2].tile = 2;
			OAM[3].tile = 3;
		}
		
		frametime = 0;
	}

	speed = 7;

	if(!(KEY_PAD&KEY_A))
	{
		speed = 12;
	}
  
	if(!(KEY_PAD&KEY_LEFT))
	{
		if (OAM[0].x > 1+16)
		{
			OAM[0].x = OAM[0].x - speed;
			OAM[1].x = OAM[1].x - speed;
			OAM[2].x = OAM[2].x - speed;
			OAM[3].x = OAM[3].x - speed;
		}
	}
	else if(!(KEY_PAD&KEY_RIGHT))
	{
		if (OAM[0].x < 95-16)
		{
			OAM[0].x = OAM[0].x + speed;
			OAM[1].x = OAM[1].x + speed;
			OAM[2].x = OAM[2].x + speed;
			OAM[3].x = OAM[3].x + speed;
		}
	}
}

int main()
{
	unsigned char i;
	
	PRC_MODE = COPY_ENABLE|SPRITE_ENABLE|MAP_ENABLE|MAP_16X12;
	
	PRC_SPR = (long)gfx_spr;
	PRC_MAP = (long)gfx_tile_frame0;
	
	for (i=0;i<6;i++)
	{
		OAM[i].ctrl = OAM_ENABLE;
	}
  
	OAM[0].tile = -1;
	OAM[0].x = 29;
	OAM[0].y = 62;

	drawbackground();

	while(1)
	{
		switch(gameMODE)
		{
			case 0:
				highscoreConvert();
				drawhighscore();
			  
				if((!(KEY_PAD&KEY_A)))
				{
					for (i=0;i<2;i++)
					{
						OAM[i].ctrl = OAM_ENABLE;
					}
					OAM[0].tile = 0;
					OAM[1].tile = 1;
					OAM[0].x = 35; 
					OAM[0].y = 72;
					OAM[1].x = OAM[0].x+16; 
					OAM[1].y = OAM[0].y;
					OAM[4].x = rand_a_b(8,80);
					OAM[4].y = -32;
					OAM[4].tile = 2;
					  
					OAM[5].x = rand_a_b(8,80);
					OAM[5].y = -32;
					OAM[5].tile = 3;
					PRC_MAP = (long)tiles_frame0;
					
					drawbackground();
				  
					//Delete score
					for (i=0;i<11;i++)
					{
						TILEMAP[7*16+i] = 11+7*12;
					}

					lives = 3;
					gameMODE = 1;
					enn1state = 0;
					enn1time = 0;
					enn2state = 0;
					enn2time = 0;
					score = 0;
					timegameover = 0;
				}
			break;
			case 1:
				drawscore();
				drawlife();
				scoreConvert();
				controlrainbowdash();
				  
				if (lives < 1)
				{
					PRC_MAP = (long)tiles_2_frame0;
					enn1state = 0;
					enn2state = 0;
					for (i=0;i<6;i++)
					{
						OAM[i].tile = -1;
					}
					drawbackground();
					gameMODE = 2;
				}
				
				if (enn1state == 0)
				{
					if ((OAM[0].x + 32 > OAM[4].x) && (OAM[0].x < OAM[4].x + 16) && (OAM[0].y + 10 > OAM[4].y) && (OAM[0].y < OAM[4].y + 16)) 
					{
						score = score + 100;
						speedENN1 = rand_a_b(4,10);
						OAM[4].tile = 4;
						enn1state = 1;
						enn1time = 0;
					}
					
					OAM[4].y = OAM[4].y + speedENN1;
						
					if (OAM[4].y > 64+16)
					{
						 lives--;
						 speedENN1 = rand_a_b(4,10);
						 OAM[4].y = rand_a_b(-32,-48);
						 OAM[4].x = rand_a_b(8,80);
					}
				}
				else
				{
					enn1time++;
					if (enn1time > 1)
					{
						OAM[4].tile++;
						enn1time = 0;
						if (OAM[4].tile > 5)
						{
							enn1state = 0;
							OAM[4].y = rand_a_b(-32,-48);
							OAM[4].x = rand_a_b(8,80);
							OAM[4].tile = 2;
						}
					}
				}
				  
				if (enn2state == 0)
				{
					if ((OAM[0].x + 32 > OAM[5].x) && (OAM[0].x < OAM[5].x + 16) && (OAM[0].y + 10 > OAM[5].y) && (OAM[0].y < OAM[5].y + 16)) 
					{
						score = score + 100;
						speedENN2 = rand_a_b(4,10);
						OAM[5].tile = 4;
						enn2state = 1;
						enn2time = 0;
					}
					
					OAM[5].y = OAM[5].y + speedENN2;	
					if (OAM[5].y > 64+16)
					{
						lives--;
						speedENN2 = rand_a_b(4,10);
						OAM[5].y = rand_a_b(-32,-48);
						OAM[5].x = rand_a_b(8,80);
					}
				}
				else
				{
					enn2time++;
					if (enn2time > 1)
					{
						OAM[5].tile++;
						enn2time = 0;
						if (OAM[5].tile > 5)
						{
							enn2state = 0;
							OAM[5].tile = 3;
							OAM[5].y = rand_a_b(-32,-48);
							OAM[5].x = rand_a_b(8,80);
						}
					}
				}
			break;
			default:
				scoreConvert();
			  
				if (timegameover > 65)
				{
					if (score > highscore) highscore = score;
					PRC_MAP = (long)gfx_tile_frame0;
					gameMODE = 0;
				}		  
				else
				{
					timegameover++;
				}
			break;
		  
		}
	}
}

void scoreConvert()
{
	scored[0] = (score / 10000) % 10;
	scored[1] = (score / 1000) % 10;
	scored[2] = (score / 100) % 10;
	scored[3] = (score / 10) % 10;
	scored[4] = score % 10;
}

void highscoreConvert()
{
	hscored[0] = (highscore / 10000) % 10;
	hscored[1] = (highscore / 1000) % 10;
	hscored[2] = (highscore / 100) % 10;
	hscored[3] = (highscore / 10) % 10;
	hscored[4] = highscore % 10;
}
