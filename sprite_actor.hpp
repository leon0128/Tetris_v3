#pragma once

#include "actor.hpp"
#include <SDL2/SDL.h>

class SpriteActor : public Actor
{
public:
    SpriteActor(class Game* game, int order);
    virtual ~SpriteActor();

    virtual void draw(SDL_Renderer* renderer);

    // メンバ変数の設定、取得
    void setTexture(SDL_Texture* texture){mTexture = texture;}

protected:
    // SDL関連
    SDL_Texture* mTexture;
    SDL_Rect mRectangle;
    // Textureの大きさ
    int mTextureWidth;
    int mTextureHeight;
};