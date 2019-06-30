#include "controller.hpp"
#include "back_ground.hpp"
#include "game_board.hpp"

Controller::Controller(Game* game, int order):
    Actor(game, order),
    mState(TITLE),
    mBackGround(nullptr),
    mGameBoard(nullptr)
{
    createGameBoard();
}

void Controller::update()
{
    if(mState == RESULT)
    {
        delete mBackGround;
        delete mGameBoard;
        mState = TITLE;
    }
}

void Controller::gameover()
{
    mState = RESULT;
}

void Controller::createGameBoard()
{
    // GameBoardを作成
    mBackGround = new BackGround(mGame, 10);
    mGameBoard = new GameBoard(mGame, 30);
    mState = PLAYING;
}