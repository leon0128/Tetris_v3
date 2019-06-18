#include "sprite_actor.hpp"

SpriteActor::SpriteActor(Game* game, int order):
    Actor(game, order),
    mTexture(nullptr),
    mTextureWidth(0),
    mTextureHeight(0),
    mClear(1.0f)
{
    mGame->addSpriteActor(this);
}

SpriteActor::~SpriteActor()
{
    mGame->removeSpriteActor(this);
}

void SpriteActor::draw(SDL_Renderer* renderer)
{
    // mTextureがnullptrでないなら描画
    if(mTexture)
    {
        SDL_RenderCopy(renderer,
                       mTexture,
                       nullptr,
                       &mRectangle); 
    }
}