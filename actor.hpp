#pragma once

#include "game.hpp"
#include "vector2.hpp"

class Actor
{
public:
    // コンストラクタでGame::mComponentActorに自身を追加
    // デストラクタでGame::mCompenentActorから自身を削除
    Actor(class Game* game, int order);
    virtual ~Actor();

    virtual void update(){};

    // メンバ変数の設定、取得
    int getOrder(){return mOrder;}

protected:
    // 自身が所属するゲーム
    class Game* mGame;
    // 更新順序
    int mOrder;
};