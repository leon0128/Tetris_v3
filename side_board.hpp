#pragma once

#include "sprite_actor.hpp"

class SideBoard : public SpriteActor
{
public:
    SideBoard(class Game* game,
              int order,
              class GameBoard* gameBoard);

    void draw(SDL_Renderer* renderer) override;
    
    // 表示するテトロミノを設定
    void setType(EType type);
    EType getType(){return mType;}

private:
    class GameBoard* mGameBoard;
    EType mType;
    
    // 表示するテトロミノ用
    SDL_Texture* mTetrominoTexture;
    SDL_Rect mTetrominoRectangle;
};