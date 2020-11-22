#ifndef EMENU_ENTRY_C
#define EMENU_ENTRY_C

#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "sdl_init_and_halt.c"

struct Entry *
make_entry(int x, int y, int w, int h, SDL_Texture *image, char *return_string)
{
	SDL_Rect *proportions = malloc(sizeof(SDL_Rect));
	if(proportions == NULL) {
		fputs("Failed to allocate memory for SDL_Rect *proportions\n", stderr);
		halt_sdl(1);
	}
	proportions->x = x;
	proportions->y = y;
	proportions->h = h;
	proportions->w = w;
	struct Entry *entry = malloc(sizeof(struct Entry*));
		if(proportions == NULL) {
		fputs("Failed to allocate memory for struct Entry *entry\n", stderr);
		halt_sdl(1);	
	}
	entry->proportions = proportions;
	entry->image = image;
	entry->return_string = return_string;
	return entry;
}

void
free_entry(struct Entry *entry)
{
	SDL_DestroyTexture(entry->image);
	free(entry->proportions);
	free(entry);
}

#endif /* EMENU_ENTRY_C */