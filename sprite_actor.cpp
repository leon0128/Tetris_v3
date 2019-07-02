#include "sprite_actor.hpp"

SpriteActor::SpriteActor(Game* game, int order):
    Actor(game, order),
    mTexture(nullptr),
    mClear(1.0f),
    mScale(1.0f)
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
        // 透明の処理
        SDL_SetTextureAlphaMod(mTexture, 
                               static_cast<int>(mClear * 255));

        // 描画
        SDL_RenderCopy(renderer,
                       mTexture,
                       nullptr,
                       &mRectangle); 
    }
}

void SpriteActor::updateRectangle()
{
    mRectangle.w = mTextureSize.x;
    mRectangle.h = mTextureSize.y;
    mRectangle.x = mPosition.x - mTextureSize.x / 2;
    mRectangle.y = mPosition.y - mTextureSize.y / 2;    
}

void SpriteActor::setTexture(SDL_Texture* texture)
{
    // mTextureの設定
    mTexture = texture;

    if(!mTexture)
    {
        return ;
    }
    
    int width, height;
    // mRectangle, mTextureSizeの初期化
    SDL_QueryTexture(mTexture,
                     nullptr,
                     nullptr,
                     &width,
                     &height);
    
    mTextureSize.set(width, height);
    mTextureSize = mTextureSize * mScale;
    updateRectangle();
}

void SpriteActor::setScale(float scale)
{
    // 描画する際に使用するそれぞれの情報を変更
    mTextureSize = mTextureSize * (scale / mScale);
    updateRectangle();
    mScale = scale;
}