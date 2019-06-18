#include "sprite_actor.hpp"

SpriteActor::SpriteActor(Game* game, int order):
    Actor(game, order),
    mTexture(nullptr)
{
    mGame->addSpriteActor(this);
}

SpriteActor::~SpriteActor()
{
    mGame->removeSpriteActor(this);
    mGame->removeA(this);
}