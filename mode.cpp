#include "mode.hpp"

#include "menu.hpp"

Mode::Mode(Game* game, EMode mode, int order):
    SpriteActor(game, order),
    mMode(mode),
    mIsSelected(false)
{
    setTextureFromMode();
}

void Mode::draw(SDL_Renderer* renderer)
{
    SpriteActor::draw(renderer);

    // 影の設定
    if(mIsSelected)
    {
        SDL_Rect rectangle = {mPosition.x - mTextureSize.x / 2 + 5,
                              mPosition.y - mTextureSize.y / 2 + 5,
                              mTextureSize.x - 10,
                              mTextureSize.y - 10};
        SDL_SetRenderDrawColor(renderer,
                               0,
                               0,
                               0,
                               50);
        SDL_RenderFillRect(renderer, &rectangle);
    }
}

void Mode::setTextureFromMode()
{
    switch (mMode)
    {
        case (MARATHON):
            mPosition.set(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4);
            setTexture(mGame->getTexture("image/mode/marathon.png"));
            setClear(0.75f);
            break;
        
        default:
            SDL_Log("This mode is not implementated %d: %s", mMode, __PRETTY_FUNCTION__);
            break;
    }
}