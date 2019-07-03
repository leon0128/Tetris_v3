#include "mode.hpp"

#include "menu.hpp"

Mode::Mode(Game* game, EMode mode, int order):
    SpriteActor(game, order),
    mMode(mode)
{
    setTextureFromMode();
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