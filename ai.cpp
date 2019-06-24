#include "ai.hpp"

std::vector<std::array<bool, GAMEBOARD_PARALLEL>> AI::mVirtualGameState;
Actor::EType AI::mActiveTetromino = NONE;
Actor::EType AI::mHoldTetromino = NONE;
std::vector<Actor::EType> AI::mNextTetromino;
bool AI::mIsCalculating = false;
struct AI::Result AI::mResult;

void AI::startCalculation(EType active, 
                          EType hold, 
                          std::vector<EType> next,
                          std::vector<std::array<Block*, GAMEBOARD_PARALLEL>> gameState)
{
    mIsCalculating = true;

    mActiveTetromino = active;
    mHoldTetromino = hold;
    mNextTetromino = next;

    mVirtualGameState.clear();
    // mVirtualGameStateの設定
    for(int y = 0; y < (int)gameState.size(); y++)
    {
        std::array<bool, GAMEBOARD_PARALLEL> line;
        mVirtualGameState.push_back(line);
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                mVirtualGameState[y][x] = true;
            }
            else
            {
                mVirtualGameState[y][x] = false;
            }
        }
    }
    
    // 別スレッドで計算実行
}