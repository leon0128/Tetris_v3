#pragma once

#include "sprite_actor.hpp"

class Pause : public SpriteActor
{
public:
    Pause(class Game* game, 
          int order = 150);
    ~Pause();

    void update() override;
    void draw(SDL_Renderer* renderer) override;

    void addGameBoard(class GameBoard* gameBoard);

private:
    // Menuに描画する文字列テクスチャの作成
    void createTexturePair();

    // 現在作成されているGameBoardクラス
    std::vector<class GameBoard*> mGameBoardVector;
    // Menuに描画する文字列の配列
    std::vector<std::pair<SDL_Texture*, SDL_Rect>> mTexturePairVector;
    // 全てのGameBoardを更新するか
    bool mIsAllUpdatedCurrent;
    bool mIsAllUpdatedBefore;
    // gameoverかどうか
    bool mIsGameover;
    // mTexturePairVectorのどれを選択しているか
    int mSelectIndex;
};