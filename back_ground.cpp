#include "back_ground.hpp"

BackGround::BackGround(Game* game, int order):
    SpriteActor(game, order)
{
    mPosition.set(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/back_ground_02.png"));
}