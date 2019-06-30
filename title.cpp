#include "title.hpp"

Title::Title(Game* game, int order):
    SpriteActor(game, order)
{
    mPosition.set(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/title.png"));
}