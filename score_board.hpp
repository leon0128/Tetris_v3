#pragma once

#include "sprite_actor.hpp"

class ScoreBoard : public SpriteActor
{
public:
    ScoreBoard(class Game* game,
               int order,
               class GameBoard* gameBoard);

    void update() override;
    void draw(SDL_Renderer* renderer) override;

private:
    // 文字列テクスチャと描画用SDL_Rectを紐付けておく
    struct TextureAndRectangle
    {
        SDL_Texture* texture;
        SDL_Rect rectangle;
    };
    
    // Description用のテクスチャを作成しmDescriptionTextureに格納
    void createDescriptionTexture();
    std::vector<struct TextureAndRectangle> mDescriptionTexture;
    // 実際にScoreを表示するTexture
    void createScoreTexture();
    std::vector<struct TextureAndRectangle> mScoreTexture;

    // 描画するフォントの色
    SDL_Color mColor;
    class GameBoard* mGameBoard;
};