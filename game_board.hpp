#pragma once

#include "sprite_actor.hpp"
#include "npc.hpp"

// 固定されるまでの時間と落下時間
// === FIX_COUNT >= DROP_COUNT ===
const int FIX_COUNT = 60;
const int DROP_COUNT = 60;
// 長押し判定までのフレームと移動速度
const int KEEPED_FRAME = 15;
const int MOVE_FRAME = 5;
// NextBoardの数
const int NEXT_SIZE = 5;

class GameBoard : public SpriteActor
{
public:
    GameBoard(class Game* game, int order = 30);
    ~GameBoard();

    // いくつかの関数の実行
    void update() override;

    // mGameStateの取得
    auto getGameState(){return mGameState;}
    // Scoreの取得
    auto getScore(){return mScores;}
    // mUpdateFrameの取得
    const int getUpdateFrame() const {return mUpdateFrame;}

    void changeIsUpdated(const bool isUpdate) {mIsUpdated = isUpdate;}
    const bool isGameover() const {return mIsGameover;}
    void gameover();
 
private:
    // 入力処理
    void inputKeyboard();
    // NPCが出した計算結果を処理
    void inputNPC();
    // mActiveTetrominoがnullptrの場合、新しいミノを作成
    void pickTetromino();
    // Hold操作
    void hold();
    // ミノの並行指導と回転移動を処理する
    void updateActiveTetromino();
    // ミノの固定、mGameStateの更新
    void updateGameState();
    // mGameStateに格納されているミノの位置を更新
    void updateBlockPosition();
    // Scoreの更新
    void updateScore();

    // GameBoardが保持する各オブジェクトの初期化
    void initializeHoldBoard();
    void initializeNextBoard();
    void initializeScoreBoard();

    // ブロックを格納する配列
    std::vector<std::array<class Block*, GAMEBOARD_PARALLEL>> mGameState;
    // 操作するTetromino
    class Tetromino* mActiveTetrominio;
    // 7種のミノのタイプを一時的に格納する配列
    std::vector<EType> mPendingTetromino;

    // Game::mKeyboardStateを格納する配列
    std::vector<int> mKeyboardState;

    // 更新フレーム数
    int mUpdateFrame;
    // GameBoardを更新するかどうか
    bool mIsUpdated;
    // gameoverかどうか
    bool mIsGameover;

    // next用のSideBoard配列
    std::array<class SideBoard*, NEXT_SIZE> mNextBoard = {nullptr};
    // hold用のSideBoardの設定
    class SideBoard* mHoldBoard;
    // holdを実行したかどうか
    bool mIsHolded;
    // ScoreBoard
    class ScoreBoard* mScoreBoard;
    // 各スコアを格納する配列
    std::array<std::string, 8> mScores;

    // NPC関連
    // NPCの計算結果を格納
    struct NPC::Result mNPCResult;
    // 結果を初期化したか
    bool mIsInitializeResult;
    // NPCの演算を処理するスレッド
    std::thread mNPCThread;
};