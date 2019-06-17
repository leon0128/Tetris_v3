#pragma once

// SDL関連
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// C++標準ライブラリ

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

private:
    // 入力処理
    void inputProcess();
    // ゲーム更新
    void updateGame();
    // 映像出力
    void outputProcess();

    // SDL関連の初期化
    bool initializeSDL();
    // SDL関連の削除
    void finalizeSDL();

    // ゲームを続けるか
    bool mIsContinuedGame;

    // SDL関連
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;
};