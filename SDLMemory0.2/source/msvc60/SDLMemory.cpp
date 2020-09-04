/* SDLMemory.cpp - Main function

Copyright 2003,2004 Aaron Ponti

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

/* Initialize all needed SDL surfaces */
SDL_Surface *screen;
SDL_Surface *title;
SDL_Surface *bkg;
SDL_Surface *back;
SDL_Surface *front[CARDS];
SDL_Surface *scorePanels[10];
SDL_Surface *scoreTitle;
SDL_Surface *newHiscoreTitle;
SDL_Surface *hiscoreTitle;

/* Initialize card positions (coords of the upper-left corner) */
int x[9]; /* The ninth card is just there to facilitate the checks below */
int y[9];

/* Initialize card status (turned or not turned) */
int status[CARDS];

/* Initialize array for card sequence */
int permute[CARDS];
int *p_perm=&permute[0];

/* Initialize hiscore */
int hiscore=0;
int *p_hiscore=&hiscore;

/* Program entry point */
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{

	/* Try to read hiscore from disk: hiscore.dat */
	int failure;
	failure=hiscoreFile(1, p_hiscore);
	if (failure==1)
	{
		/* Reset hiscore */
		*p_hiscore=0;

		/* Reset file - if this fails, the program will exit with flag=1 */
		failure=hiscoreFile(2, p_hiscore);
	}

	/* Initialize card positions (top-left corner) */
	for (int i=0;i<9;i++)
	{

		x[i]=90+i*80;
		y[i]=20+i*70;
	}

	/*************************************************************************************************************
	*
	*	  SDL initialization
	*
	**************************************************************************************************************/

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		MessageBox(NULL, "Unable to init SDL", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}
	atexit(SDL_Quit);

	/* Change window title */
	SDL_WM_SetCaption("SDLmemory v0.2 - Copyright Aaron Ponti, 2003,2004 - http://www.gpres.org","");

	/* Set video mode to 800x600 */
	screen=SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL )
	{
		MessageBox(NULL, "Unable to set 800x600 video", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}

	
	/*************************************************************************************************************
	*
	*	  Load all images
    *
	**************************************************************************************************************/

	/* Load title img */
	title=IMG_Load("img/title.png");
	if (title==NULL)
	{
		MessageBox(NULL, "Unable to load title image", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}

	/* Load background img */
	bkg=IMG_Load("img/bkgrd.png");
	if (bkg==NULL)
	{
		MessageBox(NULL, "Unable to load background image", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}

	/* Load image for card back */
	back=IMG_Load("img/card_back.png");
	if (back==NULL)
	{
		MessageBox(NULL, "Unable to load card (back) image", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}

	/* Load images for card front */
	loadCardFronts(front);

	/* Load images for score panels */
	loadScorePanels(scorePanels);

	/* Load image for score title */
	scoreTitle=IMG_Load("img/scoreTitle.png");
	if (scoreTitle==NULL)
	{
		MessageBox(NULL, "Unable to load score title image", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}

	/* Load image for new hiscore title */
	newHiscoreTitle=IMG_Load("img/newHiscoreTitle.png");
	if (newHiscoreTitle==NULL)
	{
		MessageBox(NULL, "Unable to load new hiscore title image", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}
	
	/* Load image for hiscore title */
	hiscoreTitle=IMG_Load("img/hiscoreTitle.png");
	if (hiscoreTitle==NULL)
	{
		MessageBox(NULL, "Unable to load hiscore title image", "Error", MB_ICONERROR | MB_OK);
		exit(1);
	}


	int done=1;
	while (done==1)
	{
		/*************************************************************************************************************
		*
		*	  Display title
		*
		**************************************************************************************************************/

		displayTitle(screen,title);

		/*************************************************************************************************************
		*
		*	  Display game board
		*
		**************************************************************************************************************/

		displayBoard(screen,back,bkg,x,y);

		/*************************************************************************************************************
		*
		*	  Start game
		*
		**************************************************************************************************************/

		done=mainLoop(p_perm,x,y,permute,status,back,screen,front,scorePanels,scoreTitle,newHiscoreTitle,hiscoreTitle,p_hiscore);

	}

	/* Clean memory */
	cleanMemory(title,bkg,back,scoreTitle,newHiscoreTitle,hiscoreTitle,front,scorePanels);
	
	/* Return */
	return 0;
}
