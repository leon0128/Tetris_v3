#pragma once

#include "game.hpp"
#include <SDL2/SDL.h>

class Actor
{
public:
    // コンストラクタでGame::mComponentActorに自身を追加
    // デストラクタでGame::mCompenentActorから自身を削除
    Actor(class Game* game, int order);
    virtual ~Actor();

    // メンバ変数の設定、取得
    int getOrder(){return mOrder;}
    void setTexture(SDL_Texture* texture){mTexture = texture;}

protected:
    // 自身が所属するゲーム
    class Game* mGame;
    // 更新順序
    int mOrder;
    // テクスチャ
    SDL_Texture* mTexture;
};