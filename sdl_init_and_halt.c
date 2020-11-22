#ifndef SDL_INIT_AND_HALT
#define SDL_INIT_AND_HALT

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "def.h"

void
init_sdl(int window_height, int window_width)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		REPORT_ERROR("SDL_Init");
		exit(1);
	}
	win = SDL_CreateWindow(PROGRAM_NAME " " VERSION,
	                       SDL_WINDOWPOS_CENTERED,
	                       SDL_WINDOWPOS_CENTERED,
	                       window_width,
	                       window_height,
	                       0);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if(win == NULL || ren == NULL) {
		REPORT_ERROR("win or ren are NULL");
		SDL_Quit();
		exit(1);
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
}

void
halt_sdl(int exit_code)
{
	IMG_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	exit(exit_code);
}

#endif /* SDL_INIT_AND_HALT */