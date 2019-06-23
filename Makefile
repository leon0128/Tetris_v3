CC = g++
CFLAGS = -g3 -Wall `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lpthread

MAIN = main.cpp game.cpp
ACTOR = actor.cpp sprite_actor.cpp back_ground.cpp game_board.cpp tetromino.cpp block.cpp side_board.cpp score_board.cpp ai.cpp leon_ai.cpp

main:
	$(CC) $(MAIN) $(ACTOR) $(CFLAGS) -o main

clean:
	rm -f main
