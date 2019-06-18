#include "sprite_actor.hpp"

SpriteActor::SpriteActor(Game* game, int order):
    Actor(game, order),
    mTexture(nullptr),
    mTextureWidth(0),
    mTextureHeight(0)
{
    mGame->addSpriteActor(this);
}

SpriteActor::~SpriteActor()
{
    mGame->removeSpriteActor(this);
    mGame->removeA(this);
}