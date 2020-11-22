#ifndef EMENU_DEF_H
#define EMENU_DEF_H

#include <SDL2/SDL.h>

#define PROGRAM_NAME "EMENU"
#define VERSION "0.2"
#define DEBUG_PRINT printf("reached here\n")
#define REPORT_ERROR(when) \
do { \
	fprintf(stderr, "An error happened in %s: %s\n", when, SDL_GetError()); \
	fflush(stdout); \
} while(0)


SDL_Window *win;
SDL_Renderer *ren;
SDL_Event eve;

struct Entry {
	SDL_Rect *proportions;
	SDL_Texture *image;
	char *return_string;
};

void init_sdl(int window_height, int window_width);
void halt_sdl(int exit_code);
struct Entry* make_entry(int x, int y, int w, int h, SDL_Texture *image, char *return_string);
void free_entry(struct Entry *entry);
SDL_Texture *get_texture_from_filename(char *name);
int mousedown(struct Entry **entries, int entriesc);
void run(struct Entry **entries, int entriesc, int r, int g, int b);
void help(void);


#endif /* EMENU_DEF_H */