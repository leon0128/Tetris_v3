#pragma once

#include "sprite_actor.hpp"

class Block : public SpriteActor
{
public:
    Block(class Game* game,
          int order,
          class GameBoard* gameBoald,
          class Vector2 coordinate);

    // 座標を元にmGameBoardに対する適切な位置を計算
    void updatePosition();

    // mCoordinateの設定、取得
    class Vector2 getCoordinate(){return mCoordinate;}
    void setCoordinate(class Vector2 coordinate){mCoordinate = coordinate;}

private:
    class GameBoard* mGameBoard;
    class Vector2 mCoordinate;
};