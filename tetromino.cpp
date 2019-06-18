#include "tetromino.hpp"

Tetromino::Tetromino(Game* game, int order, GameBoard* gameBoard):
    SpriteActor(game, order),
    mGameBoard(gameBoard)
{

}