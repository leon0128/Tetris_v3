CC            = g++
CFLAGS        = -g3 -Wall 
LIBS          = `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lpthread
OBJS          = main.o game.o actor.o sprite_actor.o back_ground.o game_board.o tetromino.o block.o side_board.o score_board.o npc.o
PROGRAM       = main

all:            $(PROGRAM)

$(PROGRAM):	$(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:;         rm -f *.o *~ $(PROGRAM)
