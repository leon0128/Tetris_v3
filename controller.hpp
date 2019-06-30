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
    void changeMode(EState state){mCurrentState = state;};

private:
    void createTitle();
    void createGameBoard();

    // gameの状態
    EState mCurrentState;
    EState mBeforeState;
    
    // ゲームの状態と作成したクラスのポインタ
    std::vector<std::pair<EState, class Actor*>> mPairVector;
};