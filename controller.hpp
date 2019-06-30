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

    // gameover時に呼び出す
    void gameover();

private:
    void createGameBoard();

    // gameの状態
    EState mState;
    
    // ゲームの進行に必要な各クラスのポインタ
    class BackGround* mBackGround;
    class GameBoard* mGameBoard;
};