#pragma once

#include "actor.hpp"

class SpriteActor : public Actor
{
public:
    SpriteActor(class Game* game, int order);
    virtual ~SpriteActor();

    // 描画のみ
    virtual void draw(SDL_Renderer* renderer);

    // 描画するテクスチャの設定
    // mRectangleの初期化
    void setTexture(SDL_Texture* texture);
    // 透明度の変更
    void setClear(float clear){mClear = clear;}

protected:
    // SDL関連
    SDL_Texture* mTexture;
    SDL_Rect mRectangle;
    // Textureの大きさ
    class Vector2* mTextureSize;
    // 中心位置
    class Vector2* mPosition;
    // 透明度
    float mClear;
};