#pragma once

#include "actor.hpp"

class Controller : public Actor
{
public:
    Controller(class Game* game,
               int order = 0);

    enum EState
    {
        TITLE,
        PLAYING,
        RESULT
    };

    // フラグに応じて何をするか決める
    void update() override;

    // Gameの状態を変更する時に呼び出す
    void changeMode(EState state){mCurrentState = state;}
    // 現在のモードを初期化する
    void initializeMode();

private:
    // 引数の状態に必要なオブジェクトの生成
    void createGameActor(EState state);

    // gameの状態
    EState mCurrentState;
    EState mBeforeState;
    
    // ゲームの状態と作成したクラスのポインタ
    std::vector<std::pair<EState, class Actor*>> mPairVector;
};