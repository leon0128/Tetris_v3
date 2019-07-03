#include "mode.hpp"

#include "menu.hpp"

Mode::Mode(Game* game, EMode mode, int order):
    SpriteActor(game, order),
    mMode(mode)
{

}