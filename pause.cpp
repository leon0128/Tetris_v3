#include "pause.hpp"

#include "game_board.hpp"

Pause::Pause(Game* game, int order):
    SpriteActor(game, order),
    mIsAllUpdated(true)
{
    mPosition.set(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/score_board.png"));
    setClear(0.75f);
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

        SpriteActor::draw(renderer);
    }
}

void Pause::addGameBoard(GameBoard* gameBoard)
{
    mGameBoardVector.push_back(gameBoard);
}

void Pause::createTexturePair()
{
    SDL_Color color = {0x00, 0x00, 0x00};

    std::vector<std::string> message;
    message.push_back("ゲームに戻る");
    message.push_back(" はじめから ");
    message.push_back(" タイトルへ ");

    for(int i = 0; i < (int)message.size(); i++)
    {
        SDL_Surface* surface = TTF_RenderUTF8_Blended(mGame->getFont(),
                                                      message.at(i).c_str(),
                                                      color);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(mGame->getRenderer(),
                                                            surface);
        SDL_FreeSurface(surface);

        SDL_Rect rectangle;
        SDL_QueryTexture(texture,
                         nullptr,
                         nullptr,
                         &rectangle.w,
                         &rectangle.h);
        rectangle.x = static_cast<int>(mPosition.x - rectangle.w / 2);
        rectangle.y = static_cast<int>(mTextureSize.y / message.size() * i);

        mTexturePairVector.emplace_back(texture, rectangle);
    }
}