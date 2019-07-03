#pragma once

#include "sprite_actor.hpp"

class ScoreBoard : public SpriteActor
{
public:
    ScoreBoard(class Game* game,
               int order,
               class GameBoard* gameBoard);
    ~ScoreBoard();

    void draw(SDL_Renderer* renderer) override;

private:
    // 終了時にScoreをコンソールに出力
    void printScore();

    // Description用のテクスチャを作成しmDescriptionTextureに格納
    void createDescriptionTexture();
    std::vector<std::pair<SDL_Texture*, SDL_Rect>> mDescriptionPairVector;
    // scoreを描画する
    void createMap();
    void drawScore(SDL_Renderer* renderer);
    std::unordered_map<char, SDL_Texture*> mScoreTexture;

    // 描画するフォントの色
    SDL_Color mColor;
    class GameBoard* mGameBoard;
};