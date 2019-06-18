#pragma once

#include "actor.hpp"
#include <SDL2/SDL.h>

class SpriteActor : public Actor
{
public:
    SpriteActor(class Game* game, int order);
    ~SpriteActor();

    // メンバ変数の設定、取得
    void setTexture(SDL_Texture* texture){mTexture = texture;}

protected:
    SDL_Texture* mTexture;
};