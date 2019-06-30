#include "controller.hpp"
#include "back_ground.hpp"
#include "game_board.hpp"

Controller::Controller(Game* game, int order):
    Actor(game, order),
    mState(TITLE)
{
    createGameBoard();
}

void Controller::update()
{
    if(mState == RESULT)
    {
        auto iterator = mPairVector.begin();
        while(iterator != mPairVector.end())
        {
            if(iterator->first == "GameBoard" ||
               iterator->first == "BackGround")
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
        mState = TITLE;
    }
}

void Controller::createGameBoard()
{
    // GameBoardを作成
    mPairVector.emplace_back("BackGround", new BackGround(mGame, 10));
    mPairVector.emplace_back("GameBoard", new GameBoard(mGame, 30));
    mState = PLAYING;
}