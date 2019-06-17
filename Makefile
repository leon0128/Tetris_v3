CC = g++
CFLAGS = -g3 -Wall `sdl2-config --cflags --libs`

MAIN = main.cpp game.cpp

main:
	$(CC) $(MAIN) $(CFLAGS) -o main

clean:
	rm -f main