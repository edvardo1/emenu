#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entry.c"

#define PROGRAM_NAME "EMENU"
#define VERSION "0.2"
#define DEBUG_PRINT printf("reached here\n")
#define REPORT_ERROR(when) \
do { \
	printf("An error happened in %s: %s\n", when, SDL_GetError()); \
	fflush(stdout); \
} while(0)

SDL_Window *win;
SDL_Renderer *ren;
SDL_Event eve;


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

SDL_Texture *
get_texture_from_filename(char *name)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = NULL;

	surface = IMG_Load(name);

	if(!surface) {
		puts(IMG_GetError());
		halt_sdl(1);
	}

	texture = SDL_CreateTextureFromSurface(ren, surface);

	if(!texture) {
		REPORT_ERROR("Texture is NULL");
		halt_sdl(1);
	}

	SDL_FreeSurface(surface);

	return texture;
}

int
mousedown(struct Entry **entries, int entriesc)
{
	int i, mousex, mousey, is_over_button;
	SDL_GetMouseState(&mousex, &mousey);
	for(i = 0; i < entriesc; ++i) {
		is_over_button =
			(entries[i]->proportions->x < mousex &&
			 mousex < entries[i]->proportions->x + entries[i]->proportions->w) &&
			(entries[i]->proportions->y < mousey &&
			 mousey < entries[i]->proportions->y + entries[i]->proportions->h);
		if(is_over_button) {
			puts(entries[i]->return_string);
			return 1;
		}
	}
	return 0;
}

void
run(struct Entry **entries, int entriesc, int r, int g, int b)
{
	int i, quitting = 0;
	while(!quitting) {
		SDL_WaitEvent(&eve);
		switch(eve.type) {
		case SDL_QUIT :
			quitting = 1;
			break;
		case SDL_MOUSEBUTTONDOWN :
			quitting = mousedown(entries, entriesc);
			break;
		default : break;
		}
		if(SDL_SetRenderDrawColor(ren, r, g, b, 0) != 0) { 
			REPORT_ERROR("SDL_SetRenderDrawColor");
		}
		if(SDL_RenderClear(ren)) {
			REPORT_ERROR("SDL_RenderClear");
		}
		for(i = 0; i < entriesc; ++i) {
			if(SDL_RenderCopy(ren, entries[i]->image,
			                  NULL, entries[i]->proportions) != 0) {
				REPORT_ERROR("SDL_RenderCopy");
			}
		}
		SDL_RenderPresent(ren);
	}
}

void
help(void)
{
	printf(
		"Format:\n  "
		"emenu <Window Height> <Window Width> <Button Height> <Button Width> "
		"<Hex Background Colour> Entries\n"
		"Entries format:\n  "
		"<image> <return string>\n"
		"Example:\n  "
		"emenu 600 800 100 100 FFFFFF entry1.png entry1string"
		" entry2.png entry2string\n"
	);
}

int
main(int argc, char **argv)
{
	int resx, resy, current_x, current_y,
		button_width, button_height, i, j, background, entriesc;

	--argc, ++argv;
	/* parsing arguments */
	if(argc <  6) {
		help();
		return 0;
	}

	resx = atoi(argv[0]);
	resy = atoi(argv[1]);
	button_width = atoi(argv[2]);
	button_height = atoi(argv[3]);
	background = strtol(argv[4], NULL, 16);
	argc -= 5;
	argv += 5;
	entriesc = argc / 2;

	init_sdl(resx, resy);

	/* setup */
	struct Entry **entries = malloc(entriesc * sizeof(struct Entry*));
	if(entries == NULL) {
		puts("Malloc failed to allocate memory for entries");
		halt_sdl(0);
	}

	current_x = 0; current_y = 0;

	for(i = 0, j = 0; i < entriesc; ++i, j += 2) {
		entries[i] = make_entry(current_x, current_y,
		                        button_height, button_width,
		                        get_texture_from_filename(argv[j]),
		                        argv[j+1]);
		if(resx <= button_width + current_x) {
			current_x = 0; 
			current_y += button_height;
		} else {
			current_x += button_width;
		}
	}



	run(entries, entriesc,
	    background >> 16, /* r */
	    (background >> 8) & 0xFF, /* g */
	    background & 0xFF); /* b */

	/* cleanup */
	for(i = 0; i < entriesc; ++i) {
		free_entry(entries[i]);
	}
	free(entries);

	halt_sdl(0);
	return 0;
}