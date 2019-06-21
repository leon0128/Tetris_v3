#include "side_board.hpp"

SideBoard::SideBoard(Game* game, int order, GameBoard* gameBoard):
    SpriteActor(game, order),
    mGameBoard(gameBoard),
    mType(NONE),
    mTetrominoTexture(nullptr)
{
    setTexture(mGame->getTexture("image/side_board.png"));
    setClear(0.65f);
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
            SDL_Log("Can not set Tetromino texture: SideBoard::setType()");
            break;
    }

    if(mTetrominoTexture)
    {
        int width, height;
        // mRectangle, mTextureSizeの初期化
        SDL_QueryTexture(mTexture,
                        nullptr,
                        nullptr,
                        &width,
                        &height);
        
        mRectangle.w = width;
        mRectangle.h = height;
        mRectangle.x = mPosition.x - width / 2;
        mRectangle.y = mPosition.y - height / 2;   
    }
}