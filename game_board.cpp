#include "game_board.hpp"

GameBoard::GameBoard(Game* game, int order):
    SpriteActor(game, order)
{
    // 自身の位置とテクスチャの設定
    mPosition = new Vector2(WINDOW_WIDTH * 3 / 8, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/game_board.png"));
}

void GameBoard::update(int cuurentFrame)
{
    
}