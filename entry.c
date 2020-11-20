#ifndef EMENU_ENTRY_C
#define EMENU_ENTRY_C

struct Entry {
	SDL_Rect *proportions;
	SDL_Texture *image;
	char *return_string;
};

struct Entry *
make_entry(int x, int y, int w, int h, SDL_Texture *image, char *return_string)
{
	SDL_Rect *proportions = malloc(sizeof(SDL_Rect));
	proportions->x = x;
	proportions->y = y;
	proportions->h = h;
	proportions->w = w;
	struct Entry *entry = malloc(sizeof(struct Entry*));
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