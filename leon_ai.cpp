#include "leon_ai.hpp"

LeonAI::LeonAI(Game* game, int order, GameBoard* gameBoard):
    AI(game, order, gameBoard)
{
}

void LeonAI::calculate()
{
    mResult.isHoled = rand() % 2;
    mResult.direction = rand() % 4;
    mResult.coordinate = rand() % 10;
    mIsCalculating = false;
}