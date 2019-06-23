#include "ai.hpp"

AI::AI(Game* game, int order, GameBoard* gameBoard):
    Actor(game, order),
    mGameBoard(gameBoard),
    mIsCalculating(true)
{
}

void AI::startCalculation()
{
    // mResultの初期化
    mIsCalculating = true;
    mResult.direction = UP;
    mResult.coordinate = 4;
    mResult.isHoled = false;
}

void AI::endCalculation()
{
    mIsCalculating = false;
}