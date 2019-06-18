#include "sprite_actor.hpp"

SpriteActor::SpriteActor(Game* game, int order):
    Actor(game, order),
    mTexture(nullptr),
    mTextureSize(0, 0),
    mPosition(0, 0),
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

void SpriteActor::setTexture(SDL_Texture* texture)
{
    // mTextureの設定
    mTexture = texture;

    // mRectangle, mTextureSizeの初期化
    SDL_QueryTexture(mTexture,
                     nullptr,
                     nullptr,
                     &mTextureSize.x,
                     &mTextureSize.y);
    mRectangle.w = mTextureSize.x;
    mRectangle.h = mTextureSize.y;
    mRectangle.x = mPosition.x - mTextureSize.x / 2;
    mRectangle.y = mPosition.y - mTextureSize.y / 2;
}