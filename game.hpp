#pragma once

// SDL関連
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// C++, C標準ライブラリ
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <array>
#include <cstring>
#include <thread>
#include <cmath>
#include <limits>
#include <time.h>
#include <unistd.h>

// ウィンドウの幅と高さ
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
// ゲームボードのブロックの大きさ
const int BLOCK_SIZE = 35;
// ゲームボードの縦と横のブロック数
const int GAMEBOARD_PARALLEL = 10;
const int GAMEBOARD_VERTICAL = 25;

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

    // 引数のファイルパスのテクスチャを取得する
    SDL_Texture* getTexture(const std::string& filename);

    // mCompoentActorに要素を出し入れする関数
    void addActor(class Actor* actor);
    void removeActor(class Actor* actor);
    // mComponentSpriteActorに要素を出し入れ
    void addSpriteActor(class SpriteActor* actor);
    void removeSpriteActor(class SpriteActor* actor);

    // 現在のフレームを取得
    int getFrameCount(){return mFrameCount;}
    // mKeyboardStateを返す
    auto getKeyboardState(){return mKeyboardState;}
    // フォントとレンダラーの取得
    SDL_Renderer* getRenderer(){return mRenderer;}
    TTF_Font* getFont(){return mFont;}

    // ゲーム終了の呼び出し
    void gameover(){mIsContinuedGame = false;}

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
    // 画像ファイルからテクスチャを作成
    void createTexture(const std::string& filename);

    // ゲームを続けるか
    bool mIsContinuedGame;
    // 60fpsに設定する用
    Uint32 mTicksCount;
    // ゲーム開始からのフレーム数
    int mFrameCount;

    // Actorクラスは作成時にここに追加される
    std::vector<class Actor*> mComponentActor;
    // SpriteActorクラス用
    std::vector<class SpriteActor*> mComponentSpriteActor;
    // テクスチャのファイルパスとポインタを格納するマップ
    std::unordered_map<std::string, SDL_Texture*> mTextureMap;

    // キーボードの状態を格納
    // キーボードが何フレーム押し続けられているか
    std::vector<int> mKeyboardState;

    // SDL関連
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;
};