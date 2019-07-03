#include "controller.hpp"
#include "back_ground.hpp"
#include "game_board.hpp"
#include "title.hpp"
#include "pause.hpp"
#include "menu.hpp"

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
            delete iterator->second;
            std::iter_swap(iterator, mPairVector.end() -1);
            mPairVector.pop_back();
        }
        
        createGameActor(mCurrentState);
    }
    
    mBeforeState = mCurrentState;
}

void Controller::initializeMode()
{
    if(mBeforeState == TITLE)
    {
        mBeforeState = PLAYING;
    }
    else
    {
        mBeforeState = TITLE;
    }
}

void Controller::createGameActor(EState state)
{
    Pause* pause = nullptr;
    GameBoard* gameBoard = nullptr;
    switch(state)
    {
        case (TITLE):
            mPairVector.emplace_back(TITLE, new Title(mGame));
            break;

        case (MENU):
            mPairVector.emplace_back(MENU, new Menu(mGame));
            break;
        
        case (PLAYING):
            pause = new Pause(mGame);
            gameBoard = new GameBoard(mGame);
            pause->addGameBoard(gameBoard);
            mPairVector.emplace_back(PLAYING, pause);
            mPairVector.emplace_back(PLAYING, gameBoard);
            mPairVector.emplace_back(PLAYING, new BackGround(mGame));
            break;
        
        default:
            SDL_Log("It is an enumeration that is not defined: %s", __func__);
            break;
    }
}