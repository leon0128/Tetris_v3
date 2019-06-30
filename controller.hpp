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
    void changeMode(EState state){mState = state;};

private:
    void createGameBoard();

    // gameの状態
    EState mState;
    
    // ゲームの進行に必要な各クラスのポインタとクラス名
    std::vector<std::pair<std::string, class Actor*>> mPairVector;
};