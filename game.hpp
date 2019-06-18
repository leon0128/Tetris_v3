#pragma once

// SDL関連
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// C++標準ライブラリ
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

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

    // 引数のファイルパスのテクスチャを取得する
    SDL_Texture* getTexture(const std::string& filename);

    // mCompoentActorに要素を出し入れする関数
    void addActor(class Actor* actor);
    void removeActor(class Actor* actor);
    // mComponentSpriteActorに要素を出し入れ
    void addSpriteActor(class SpriteActor* actor);
    void removeSpriteActor(class SpriteActor* actor);
    // 削除するActorを一時的に格納
    void deletedActor(class Actor* actor){mRemoveActor.push_back(actor);}

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
    // 削除されるActorを一時的に格納
    std::vector<class Actor*> mRemoveActor;
    // SpriteActorクラス用
    std::vector<class SpriteActor*> mComponentSpriteActor;
    // テクスチャのファイルパスとポインタを格納するマップ
    std::unordered_map<std::string, SDL_Texture*> mTextureMap;

    // SDL関連
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;
};