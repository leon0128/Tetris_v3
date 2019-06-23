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
    // 文字列テクスチャと描画用SDL_Rectを紐付けておく
    struct TextureAndRectangle
    {
        SDL_Texture* texture;
        SDL_Rect rectangle;
    };
    
    // Description用のテクスチャを作成しmDescriptionTextureに格納
    void createDescriptionTexture();
    std::vector<struct TextureAndRectangle> mDescriptionTexture;
    // scoreを描画する
    void createMap();
    void drawScore(SDL_Renderer* renderer);
    std::unordered_map<char, SDL_Texture*> mScoreTexture;

    // 描画するフォントの色
    SDL_Color mColor;
    class GameBoard* mGameBoard;
};