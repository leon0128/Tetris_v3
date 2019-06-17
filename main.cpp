#include "game.hpp"

int main(int argc, char** argv)
{
    Game game;
    if(game.initialize())
    {
        game.runLoop();
    }
    game.shutdown();
    
    return 0;
}