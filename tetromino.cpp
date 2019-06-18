#include "tetromino.hpp"
#include "game_board.hpp"

Tetromino::Tetromino(Game* game, 
                     int order, 
                     GameBoard* gameBoard,
                     EType type):
    Actor(game, order),
    mGameBoard(gameBoard),
    mType(type)
{

}