#include "controller.hpp"
#include "back_ground.hpp"
#include "game_board.hpp"
#include "title.hpp"

Controller::Controller(Game* game, int order):
    Actor(game, order),
    mCurrentState(TITLE),
    mBeforeState(TITLE)
{
    createGameActor(mCurrentState);
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
        
        createGameActor(mCurrentState);
    }
    
    mBeforeState = mCurrentState;
}

void Controller::createGameActor(EState state)
{
    switch(state)
    {
        case (TITLE):
            mPairVector.emplace_back(TITLE, new Title(mGame));
            break;
        
        case (PLAYING):
            mPairVector.emplace_back(PLAYING, new GameBoard(mGame));
            mPairVector.emplace_back(PLAYING, new BackGround(mGame));
            break;
        
        default:
            SDL_Log("It is an enumeration that is not defined: %s", __func__);
            break;
    }
}