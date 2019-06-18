#include "tetromino.hpp"
#include "game_board.hpp"

Tetromino::Tetromino(Game* game, int order, GameBoard* gameBoard):
    Actor(game, order),
    mGameBoard(gameBoard)
{

}