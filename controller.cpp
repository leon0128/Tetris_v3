#include "controller.hpp"
#include "back_ground.hpp"
#include "game_board.hpp"
#include "title.hpp"

Controller::Controller(Game* game, int order):
    Actor(game, order),
    mCurrentState(TITLE),
    mBeforeState(TITLE)
{
    createGameBoard();
    mCurrentState = PLAYING;
}

void Controller::update()
{
    if(mBeforeState != mCurrentState)
    {
        auto iterator = mPairVector.begin();
        while(iterator != mPairVector.end())
        {
            if(iterator->first != mCurrentState)
            {
                delete iterator->second;
                std::iter_swap(iterator, mPairVector.end() -1);
                mPairVector.pop_back();
            }
            else
            {
                iterator ++;
            }
        }
    }
    
    mBeforeState = mCurrentState;
}

void Controller::createTitle()
{
    // titleの表示
    mPairVector.emplace_back(TITLE, new Title(mGame));
}

void Controller::createGameBoard()
{
    // GameBoardを作成
    mPairVector.emplace_back(PLAYING, new BackGround(mGame, 10));
    mPairVector.emplace_back(PLAYING, new GameBoard(mGame, 30));
}