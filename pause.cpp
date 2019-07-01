#include "pause.hpp"

#include "game_board.hpp"

Pause::Pause(Game* game, int order):
    SpriteActor(game, order),
    mIsAllUpdated(true)
{
}

void Pause::update()
{
    // 全体を更新するか停止するか確認
    if(mIsAllUpdated)
    {
        for(int i = 0; i < (int)mGameBoardVector.size(); i++)
        {
            if(!mGameBoardVector.at(i)->isUpdated())
            {
                mIsAllUpdated = false;
                break;
            }
        }
    }
    else
    {
        for(int i = 0; i < (int)mGameBoardVector.size(); i++)
        {
            if(mGameBoardVector.at(i)->isUpdated())
            {
                mIsAllUpdated = true;
                break;
            }
        }
    }

    // 全体の更新するかどうかを揃える
    for(int i = 0; i < (int)mGameBoardVector.size(); i++)
    {
        mGameBoardVector.at(i)->changeIsUpdated(mIsAllUpdated);
    }
}

void Pause::draw(SDL_Renderer* renderer)
{
    // 更新停止中のみpause画面の描写
    if(!mIsAllUpdated)
    {
        SDL_SetRenderDrawColor(renderer,
                               0,
                               0,
                               0,
                               100);
        
        SDL_Rect rectangle = {0,
                              0,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT};

        SDL_RenderFillRect(renderer, &rectangle);
        SDL_Log("paused");
    }
}

void Pause::addGameBoard(GameBoard* gameBoard)
{
    mGameBoardVector.push_back(gameBoard);
}