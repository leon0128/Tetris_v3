#include "score_board.hpp"
#include "game_board.hpp"

ScoreBoard::ScoreBoard(Game* game, int order, GameBoard* gameBaord):
    SpriteActor(game, order),
    mGameBoard(gameBaord)
{
    setTexture(mGame->getTexture("image/score_board.png"));
    setClear(0.65f);

    Vector2 position = mGameBoard->getPosition();
    position.x += 500;
    position.y += 100;
    setPosition(position);
    updateRectangle();

    // 色を黒で初期化
    mColor = {0x00, 0x00, 0x00};

    createDescriptionTexture();
}

void ScoreBoard::update()
{
    createScoreTexture();
}

void ScoreBoard::draw(SDL_Renderer* renderer)
{
    SpriteActor::draw(renderer);

    for(int i = 0; i < (int)mDescriptionTexture.size(); i++)
    {
        SDL_RenderCopy(renderer,
                       mDescriptionTexture[i].texture,
                       nullptr,
                       &mDescriptionTexture[i].rectangle);
    }
    for(int i = 0; i < (int)mScoreTexture.size(); i++)
    {
        SDL_RenderCopy(renderer,
                       mScoreTexture[i].texture,
                       nullptr,
                       &mScoreTexture[i].rectangle);
    }
}

void ScoreBoard::createDescriptionTexture()
{
    std::vector<std::string> message;
    message.push_back("    IGT     :");  
    message.push_back("   SCORE    :");
    message.push_back("DELETED LINE:");
    message.push_back("  USED MINO :");
    message.push_back("   TETRIS   :");
    message.push_back("   3 LINES  :");
    message.push_back("   2 LINES  :");
    message.push_back("   1 LINE   :");
    
    for(int i = 0; i < (int)message.size(); i++)
    {
        SDL_Surface* surface = TTF_RenderUTF8_Blended(mGame->getFont(),
                                                      message[i].c_str(),
                                                      mColor);
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(mGame->getRenderer(),
                                                            surface);
        SDL_FreeSurface(surface);

        SDL_Rect rectangle;
        SDL_QueryTexture(texture,
                         nullptr,
                         nullptr,
                         &rectangle.w,
                         &rectangle.h);
        rectangle.x = static_cast<int>(mPosition.x -
                                       mTextureSize.x / 5 -
                                       rectangle.w / 2);
                      
        rectangle.y = static_cast<int>(mPosition.y -
                                       (mTextureSize.y - 30) / 2 +
                                       mTextureSize.y / message.size() * i);

        TextureAndRectangle tar = {texture, rectangle};
        mDescriptionTexture.push_back(tar);
        
        SDL_Rect rect;
        TextureAndRectangle temp = {nullptr, rect};
        mScoreTexture.push_back(temp);
    }
}

void ScoreBoard::createScoreTexture()
{
    auto scores = mGameBoard->getScore();

    for(int i = 0; i < (int)scores.size(); i++)
    {
        SDL_Surface* surface = TTF_RenderUTF8_Blended(mGame->getFont(),
                                                      scores[i].c_str(),
                                                      mColor);
        
        mScoreTexture[i].texture = SDL_CreateTextureFromSurface(mGame->getRenderer(),
                                                                surface);
        SDL_FreeSurface(surface);

        SDL_QueryTexture(mScoreTexture[i].texture,
                         nullptr,
                         nullptr,
                         &mScoreTexture[i].rectangle.w,
                         &mScoreTexture[i].rectangle.h);
        mScoreTexture[i].rectangle.x = static_cast<int>(mPosition.x +
                                       mTextureSize.x / 2 -
                                       mScoreTexture[i].rectangle.w - 10);
                      
        mScoreTexture[i].rectangle.y = static_cast<int>(mPosition.y -
                                       (mTextureSize.y - 30) / 2 +
                                       mTextureSize.y / mScoreTexture.size() * i);
    }
}