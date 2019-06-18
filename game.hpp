#pragma once

// 自身で作成したもの
#include "actor.hpp"
// SDL関連
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// C++標準ライブラリ
#include <vector>
#include <algorithm>

// ウィンドウの幅と高さ
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
// ゲームボードのブロックの大きさ
const int BLOCK_SIZE = 35;

class Game
{
public:
    Game();

    // ゲームの初期化
    bool initialize();
    // ゲームループ
    void runLoop();
    // シャットダウン
    void finalize();

    // mCompoentActorに要素を出し入れする関数
    void addActor(class Actor* actor);
    void removeActor(class Actor* actor);
    // mComponentSpriteActorに要素を出し入れ
    void addSpriteActor(class SpriteActor* actor);

private:
    // 入力処理
    void inputProcess();
    // ゲーム更新
    void updateGame();
    // 映像出力
    void outputProcess();

    // SDL関連の初期化と終了
    bool initializeSDL();
    void finalizeSDL();
    // その他の初期化と終了
    void initializeActor();
    void finalizeActor();

    // ゲームを続けるか
    bool mIsContinuedGame;
    // Actorクラスは作成時にここに追加される
    std::vector<class Actor*> mComponentActor;
    // SpriteActorクラス用
    std::vector<class SpriteActor*> mComponentSpriteActor;

    // SDL関連
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;
};