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
    createMap();
}

ScoreBoard::~ScoreBoard()
{  
    printScore();
    for(int i = 0; i < (int)mDescriptionTexture.size(); i++)
    {
        if(!mDescriptionTexture[i].texture)
        {
            SDL_DestroyTexture(mDescriptionTexture[i].texture);
        }
    }
    mDescriptionTexture.clear();ww
    for(auto texture: mScoreTexture)
    {
        SDL_DestroyTexture(texture.second);
    }
    mScoreTexture.clear();
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
    drawScore(renderer);
}

void ScoreBoard::printScore()
{
    std::vector<std::string> message;
    auto scores = mGameBoard->getScore();
    message.push_back("    IGT     :");  
    message.push_back("   SCORE    :");
    message.push_back("DELETED LINE:");
    message.push_back("  USED MINO :");
    message.push_back("   TETRIS   :");
    message.push_back("   3 LINES  :");
    message.push_back("   2 LINES  :");
    message.push_back("   1 LINE   :");

    SDL_Log("=====RESULT=====");
    for(int i = 0; i < (int)message.size(); i++)
    {
        SDL_Log("%s %s", message[i].c_str(), scores[i].c_str());
    }
    SDL_Log("================");
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
    }
}

void ScoreBoard::createMap()
{
    // scoreの描画に必要な文字
    std::vector<std::string> temp;
    temp.push_back("0");
    temp.push_back("1");
    temp.push_back("2");
    temp.push_back("3");
    temp.push_back("4");
    temp.push_back("5");
    temp.push_back("6");
    temp.push_back("7");
    temp.push_back("8");
    temp.push_back("9");
    temp.push_back(":");
    temp.push_back(".");
    
    // textureの作成
    for(auto element : temp)
    {
        SDL_Surface* surface = TTF_RenderUTF8_Blended(mGame->getFont(),
                                                      element.c_str(),
                                                      mColor);       
        SDL_Texture* texture = SDL_CreateTextureFromSurface(mGame->getRenderer(),
                                                            surface);
        SDL_FreeSurface(surface);
        
        mScoreTexture.emplace(element.c_str()[0], texture);
    }
}

void ScoreBoard::drawScore(SDL_Renderer* renderer)
{
    // 文字１つ分の大きさを取得
    SDL_Rect rectangle;
    SDL_QueryTexture(mScoreTexture[':'],
                     nullptr,
                     nullptr,
                     &rectangle.w,
                     &rectangle.h);

    // 描画
    auto scores = mGameBoard->getScore();
    for(int i = 0; i < (int)scores.size(); i++)
    {   
        rectangle.y = static_cast<int>(mPosition.y -
                                       (mTextureSize.y - 30) / 2 +
                                       mTextureSize.y / scores.size() * i);   
        for(int j = 0; j < (int)std::strlen(scores[i].c_str()); j++)
        {
            rectangle.x = static_cast<int>(mPosition.x +
                                           mTextureSize.x / 2 - 10 -
                                           rectangle.w * (std::strlen(scores[i].c_str()) - j));
            
            SDL_RenderCopy(renderer,
                           mScoreTexture[scores[i].c_str()[j]],
                           nullptr,
                           &rectangle);
        }
    }
}