#include "ai.hpp"

AI::AI(Game* game, int order, GameBoard* gameBoard):
    Actor(game, order),
    mGameBoard(gameBoard)
{

}