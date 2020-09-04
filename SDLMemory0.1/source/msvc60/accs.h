/* accs.h - Accessory function prototypes 

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

#ifndef ACCS_H_

	#define ACCS_H_

	#include <cstdlib> /* For the exit function */
	#include "SDL/SDL.h"
	#include <ctime>
	#include <cstdio>
	#include <cmath>

	/* Define the number of card pairs */
	const int CARDS=64;

	/* Lock screen */
	void Slock(SDL_Surface *screen);

	/* Unlock screen */
	void Sulock(SDL_Surface *screen);

	/* Draw image at (0,0) */
	void DrawIMG(SDL_Surface *img, SDL_Surface *scr, int x, int y);

	/* Draw image (overloaded) */
	void DrawIMG(SDL_Surface *img, SDL_Surface *scr, int x, int y, int w, int h, int x2, int y2);

	/* Draw background */
	void DrawBG(SDL_Surface *scr,SDL_Surface *img);

	/* Compare two elements (required by qsort) */
	int compare(const void *arg1, const void *arg2);

	/* Shuffle cards */
	void shuffleCards(int *permute);

	/* Load card fronts */
	void loadCardFronts(SDL_Surface **front);

	/* Display title */
	void displayTitle(SDL_Surface *screen, SDL_Surface *title);

	/* Display game board */
	void displayBoard(SDL_Surface *screen, SDL_Surface *back, SDL_Surface *bkg, int x[], int y[]);

	/* Main game loop */
	int mainLoop(int *p_perm, int x[], int y[], int permute[], int status[], 
		SDL_Surface *back, SDL_Surface *screen, SDL_Surface *front[]);

#endif
