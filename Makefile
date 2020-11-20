compile:
	gcc -Wall -lSDL2 -lSDL2_image -o emenu emenu.c
debug:
	gcc -g -Wall -lSDL2 -lSDL2_image -o emenu emenu.c
install:
	cp ./emenu /usr/bin/emenu