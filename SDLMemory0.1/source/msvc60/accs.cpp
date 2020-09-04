/* accs.cpp - Accessory functions 

Copyright 2003 Aaron Ponti

  This file is part of SDLMemory

    SDLMemory is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    SDLMemory is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SDLMemory; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "stdafx.h"
#include "accs.h"

/* Lock screen */
void Slock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
  }
}

/* Unlock screen */
void Sulock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    SDL_UnlockSurface(screen);
  }
}

/* Draw image at (x,y) */
void DrawIMG(SDL_Surface *img, SDL_Surface *scr, int x, int y)
{
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  SDL_BlitSurface(img, NULL, scr, &dest);
}

/* Draw image (overloaded) */
void DrawIMG(SDL_Surface *img, SDL_Surface *scr, int x, int y, int w, int h, int x2, int y2)
{
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  SDL_Rect dest2;
  dest2.x = x2;
  dest2.y = y2;
  dest2.w = w;
  dest2.h = h;
  SDL_BlitSurface(img, &dest2, scr, &dest);
}

/* Draw background */
void DrawBG(SDL_Surface *scr,SDL_Surface *img)
{
  Slock(scr);
  DrawIMG(img, scr, 0, 0);
  Sulock(scr);
  SDL_Flip(scr);
}

int compare(const void *arg1, const void *arg2)
{
	return ( *(int*)arg1 - *(int*)arg2 );
}

/* Shuffle cards */
void shuffleCards(int *permute)
{

	int sequence[CARDS];
	int sequence_copy[CARDS];
	int *r = &sequence[0];

	/* Initialize random number generator */
	srand(time(NULL));
	
	/* Fill the array with random numbers */
	for (int i=0;i<CARDS;i++) 
	{ 
		sequence[i] = rand();
		sequence_copy[i] = sequence[i];
	}

	/* Sort values using Quicksort algorithm */
	qsort((void *)r,CARDS,sizeof(int),compare);

	/* Perform permutation */
	for (i=0;i<CARDS;i++)
	{
		for (int j=0;j<CARDS;j++)
		{
			if (sequence_copy[i]==sequence[j])
			{
				*(permute+i)=j;
				break;	
			}

		}
	}
}

void loadCardFronts(SDL_Surface **front)
{
	char name[15];
	int j; 
	SDL_Surface *tmp;

	for (int i=1;i<=CARDS/2;i++)
	{
		if (i<10)
			j=sprintf(name,"img/card0%d.bmp",i);
		else
			j=sprintf(name,"img/card%d.bmp",i);

		// Load corresponding card from the array of filenames
		tmp=SDL_LoadBMP(name);

		if (tmp==NULL)
		{
			MessageBox(NULL, "Unable to load card image", "Error", MB_ICONERROR | MB_OK);
			exit(1);		
		}

		// Copy loaded image into the front array
		*(front+(i-1))=tmp;
	}
}

/* Display title */
void displayTitle(SDL_Surface *screen, SDL_Surface *title)
{
	/* Draw title */
	DrawBG(screen,title);
	
	/* Start loop */
	int done=0;
		
	while(done == 0)
	{
		SDL_Event event;
		while ( SDL_PollEvent(&event) )
		{
			
			if ( event.type == SDL_QUIT )  {  done = 1;  }
			if ( event.type == SDL_KEYDOWN ) 
			{ 
				if (event.key.keysym.sym==SDLK_ESCAPE)
					done=1;
				else
					done=2; 
			}
		}
	}

	if (done==1) { exit(0); } /* End of program */
}

/* Display game board */
void displayBoard(SDL_Surface *screen, SDL_Surface *back, SDL_Surface *bkg, int x[], int y[])
{
	/* Draw background */
	DrawBG(screen,bkg);
	SDL_Flip(screen);    /* Bring the background in front */

	/* Draw the 64 cards */
	for (int i=0;i<8;i++) {
		for (int j=0;j<8;j++) {
			DrawIMG(back, screen, x[i], y[j]);
		}
	}
	SDL_Flip(screen);    /* Bring the newly drawn cards in front */
}

/* Main game loop */
int mainLoop(int *p_perm, int x[], int y[], int permute[], int status[], 
			  SDL_Surface *back, SDL_Surface *screen, SDL_Surface *front[])
{
	/* Initialize card status array */
	for (int i=0;i<CARDS;i++) 
		status[i]=0; 

	/* Shuffle cards */
	shuffleCards(p_perm);


	/* Start game loop */
	int cardx=0; int cardy=0;
	int card;
	int code;
	int indices[2] = { -1, -1 };
	int active[2]  = { -1, -1 };
	int turned=0;
	int pairs=0;
	int j;
	char msg[50];
	int c_tmp;
	int score=0;
	int moves=0;

	time_t start, stop;
	double diff;

	/* Start timer */
	time(&start);

	int done=0; 
	while(done == 0)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: 
				exit(0);
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_ESCAPE)
					exit(0);
			case SDL_MOUSEBUTTONDOWN:

				
				/* Check whether two cards have been turned */	
				if (turned==2)
				{
					if (!((indices[0]==indices[1]) && (indices[0]!=-1)))
					{
						for (int i=0;i<2;i++)
						{
							cardx=active[i] % 8;				
							cardy=(active[i]-cardx)/8;
							DrawIMG(back, screen, x[cardx], y[cardy]);					
							SDL_Flip(screen);
						}
						
						/* Change status - only if the current active cards are not a pair */
						for (i=0;i<2;i++)
						{
							c_tmp=active[i];
							status[c_tmp]=0;
						}

					}
					
					/* Reset all arrays */
					for (int i=0;i<2;i++)
					{
						active[i]=-1; 
						indices[i]=-1; 
					}
					turned=0;

				}
			
				/* Identify card */
				if (((event.button.x < x[0]) || (event.button.x > x[8])) || ((event.button.y < y[0]) || (event.button.y > y[8]))) 
				{
					cardx=-1;
					cardy=-1;
					continue;
				}
				else
				{
					cardx=-1;
					for (int i=0;i<9;i++)
					{
						if ((event.button.x > x[i]) && (event.button.x < x[i+1]))
						{
							cardx=i;
							break;
						}
					}
					cardy=-1;
					for (i=0;i<9;i++)
					{
						if ((event.button.y > y[i]) && (event.button.y < y[i+1]))
						{
							cardy=i;
							break;
						}
					}
				}
				if ((cardx!=-1) && (cardy!=-1))
				{
					/* Calculate card index */
					card=8*cardy+cardx;
			
					if (status[card]==0)
					{	
						
						/* Image corresponding to the turned card */
						code=(permute[card]+2)/2;

						/* Draw card front */
						DrawIMG(front[code-1], screen, x[cardx], y[cardy]);	
						SDL_Flip(screen);
						
						/* Update status arrays */
						active[turned]=card;
						status[card]=1;
						indices[turned]=code;
						turned=turned+1;
		
						/* Update moves */
						moves++;
						
						/* Check whether we have a pair */
						if (turned==2)
						{
							if ((indices[0]==indices[1]) && (indices[0]!=-1))	
							{

								/* We have a pair */
								pairs++;


								/* If the player won, skip the loop and display the score */
								if ((turned==2) && (pairs==CARDS/2))
								{
									/* Calculate elapsed time */
									time(&stop);
									diff=difftime(stop,start);

									/* Calculate score */
									score=int(10000*exp(-diff/100)*exp(-moves/75));

									/* Give at least one point to the player */
									if (score<1)
										score=1;
									
									/* Inform and return to the main loop */
									j=sprintf(msg,"Your score: %d!",score);
									MessageBox(NULL,msg,"Congratulations!", MB_ICONERROR | MB_OK);
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	
	return done;
}