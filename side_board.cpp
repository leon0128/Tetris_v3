#include "side_board.hpp"

SideBoard::SideBoard(Game* game, int order, GameBoard* gameBoard):
    SpriteActor(game, order),
    mGameBoard(gameBoard),
    mType(NONE),
    mTetrominoTexture(nullptr)
{
    setTexture(mGame->getTexture("image/side_board.png"));
    setClear(0.65f);
    setScale(0.7f);
}

void SideBoard::draw(SDL_Renderer* renderer)
{
    SpriteActor::draw(renderer);
    
    if(mTetrominoTexture)
    {
        // 描画
        SDL_RenderCopy(renderer,
                       mTetrominoTexture,
                       nullptr,
                       &mTetrominoRectangle);
    }
}

void SideBoard::setType(EType type)
{
    mType = type;

    switch(type)
    {
        case(I):
            mTetrominoTexture = mGame->getTexture("image/tetromino/i_mino.png");
            break;

        case(O):
            mTetrominoTexture = mGame->getTexture("image/tetromino/o_mino.png");
            break;

        case(T):
            mTetrominoTexture = mGame->getTexture("image/tetromino/t_mino.png");
            break;    

        case(L):
            mTetrominoTexture = mGame->getTexture("image/tetromino/l_mino.png");
            break;

        case(J):
            mTetrominoTexture = mGame->getTexture("image/tetromino/j_mino.png");
            break; 

        case(S):
            mTetrominoTexture = mGame->getTexture("image/tetromino/s_mino.png");
            break; 

        case(Z):
            mTetrominoTexture = mGame->getTexture("image/tetromino/z_mino.png");
            break;
        
        case(NONE):
            break;
    }

    if(mTetrominoTexture)
    {
        int width, height;
        // mRectangle, mTextureSizeの初期化
        SDL_QueryTexture(mTetrominoTexture,
                         nullptr,
                         nullptr,
                         &width,
                         &height);

        width *= 0.55f * mScale;
        height *= 0.55f * mScale;

        mTetrominoRectangle.w = width;
        mTetrominoRectangle.h = height;
        mTetrominoRectangle.x = mPosition.x - width / 2;
        mTetrominoRectangle.y = mPosition.y - height / 2;   
    }
}