#include "pause.hpp"

#include "game_board.hpp"
#include "controller.hpp"

Pause::Pause(Game* game, int order):
    SpriteActor(game, order),
    mIsAllUpdatedCurrent(true),
    mIsAllUpdatedBefore(true),
    mIsGameover(false),
    mSelectIndex(0)
{
    mPosition.set(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/pause.png"));
    setClear(0.75f);
    createTexturePair();
}

Pause::~Pause()
{
    for(int i = 0; i < (int)mTexturePairVector.size(); i++)
    {
        SDL_DestroyTexture(mTexturePairVector.at(i).first);
    }
}

void Pause::update()
{
    // mGameBoardVectorに格納しているGameBoardが1つでも更新停止した場合、
    // 全体の更新を止める
    if(mGame->getKeyboardState().at(SDL_SCANCODE_SPACE) == 1)
        mIsAllUpdatedCurrent = false;

    // gameoverになったら更新停止
    for(int i = 0; i < (int)mGameBoardVector.size(); i++)
    {
        if(mGameBoardVector.at(i)->isGameover())
        {
            mIsAllUpdatedCurrent = false;
            mIsGameover = true;
        }
    }

    // gameoverの場合は、ゲームを続けるの削除
    if(mIsGameover && !mIsAllUpdatedCurrent && mIsAllUpdatedBefore)
    {
        SDL_DestroyTexture(mTexturePairVector.at(0).first);
        mTexturePairVector.erase(mTexturePairVector.begin());
        SDL_Log("pass");
    }

    for(int i = 0; i < (int)mGameBoardVector.size(); i++)
        mGameBoardVector.at(i)->changeIsUpdated(mIsAllUpdatedCurrent);

    // 更新停止中の場合は、入力に対し、mSelectIndexに対し処理を行う
    if(!mIsAllUpdatedCurrent)
    {
        if(mGame->getKeyboardState().at(SDL_SCANCODE_W) == 1)
            mSelectIndex --;
        if(mGame->getKeyboardState().at(SDL_SCANCODE_S) == 1)
            mSelectIndex ++;
        
        if(mSelectIndex < 0)
            mSelectIndex = (int)mTexturePairVector.size() -1;
        if(mSelectIndex >= (int)mTexturePairVector.size())
            mSelectIndex = 0;
    }

    if((!mIsAllUpdatedBefore && !mIsAllUpdatedCurrent) &&
       (mGame->getKeyboardState().at(SDL_SCANCODE_J) == 1 ||
        mGame->getKeyboardState().at(SDL_SCANCODE_SPACE) == 1))
    {
        if(mIsGameover)
            mSelectIndex++;
        switch (mSelectIndex)
        {
            case (0):
                mIsAllUpdatedCurrent = true;
                break;
            
            case (1):
                mGame->getController()->initializeMode();
                break;

            case (2):
                mGame->getController()->changeMode(Controller::MENU);
                break;

            case(3):
                mGame->getController()->changeMode(Controller::TITLE);
                break;
            
            default:
                SDL_Log("Invalid value for switch statement: %s", __PRETTY_FUNCTION__);
                break;
        }
        if(mIsGameover)
            mSelectIndex --;
    }

    mIsAllUpdatedBefore = mIsAllUpdatedCurrent;
}

void Pause::draw(SDL_Renderer* renderer)
{
    // 更新停止中のみpause画面の描写
    if(!mIsAllUpdatedCurrent)
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

        SDL_SetRenderDrawColor(renderer,
                               0x80,
                               0x80,
                               0x80,
                               0xff);
        SDL_RenderFillRect(renderer, &(mTexturePairVector.at(mSelectIndex).second));

        for(int i = 0; i < (int)mTexturePairVector.size(); i++)
        {
            SDL_RenderCopy(renderer,
                           mTexturePairVector.at(i).first,
                           nullptr,
                           &(mTexturePairVector.at(i).second));
        }
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
    message.push_back(" メニューへ ");
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
        rectangle.y = static_cast<int>(mPosition.y - 
                                       mTextureSize.y / 2 +
                                       mTextureSize.y / message.size() * i +
                                       3);

        mTexturePairVector.emplace_back(texture, rectangle);
    }
}