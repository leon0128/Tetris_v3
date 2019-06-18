CC = g++
CFLAGS = -g3 -Wall `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image

MAIN = main.cpp game.cpp
ACTOR = actor.cpp

main:
	$(CC) $(MAIN) $(CFLAGS) -o main

clean:
	rm -f main