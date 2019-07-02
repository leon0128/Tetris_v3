#include "game_board.hpp"
#include "tetromino.hpp"
#include "block.hpp"
#include "side_board.hpp"
#include "score_board.hpp"

GameBoard::GameBoard(Game* game, int order):
    SpriteActor(game, order),
    mActiveTetrominio(nullptr),
    mUpdateFrame(0),
    mIsUpdated(true),
    mIsGameover(false),
    mHoldBoard(nullptr),
    mIsHolded(false),
    mScoreBoard(nullptr),
    mIsInitializeResult(false)
{
    // 自身の位置とテクスチャの設定
    mPosition.set(WINDOW_WIDTH * 3 / 10, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/game_board.png"));
    setClear(0.65f);

    // mGameStateの設定
    for(int i = 0; i < GAMEBOARD_VERTICAL; i++)
    {
        std::array<class Block*, GAMEBOARD_PARALLEL> line = {nullptr};
        mGameState.push_back(line);
    }

    // mPendingTetrominoの初期化
    if(mPendingTetromino.empty())
    {
        mPendingTetromino.push_back(I);
        mPendingTetromino.push_back(O);
        mPendingTetromino.push_back(T);
        mPendingTetromino.push_back(L);
        mPendingTetromino.push_back(J);
        mPendingTetromino.push_back(S);
        mPendingTetromino.push_back(Z);
    }

    // HoldBoardの設定
    initializeHoldBoard();
    // NextBoardの設定
    initializeNextBoard();
    // ScoreBoardnの設定
    initializeScoreBoard();

    // mNPCThreadに無名関数の設定
    mNPCThread = std::thread([]{});
}

GameBoard::~GameBoard()
{
    mNPCThread.detach();
    if(mActiveTetrominio)
    {
        for(int i = 0; i < (int)mActiveTetrominio->getBlock().size(); i++)
        {
            delete mActiveTetrominio->getBlock().at(i);
        }
    }
    delete mActiveTetrominio;
    delete mScoreBoard;
    delete mHoldBoard;
    for(int i = 0; i < (int)mNextBoard.size(); i++)
    {
        delete mNextBoard.at(i);
    }
    for(int i = 0; i < (int)mGameState.size(); i++)
    {
        for(int j = 0; j < (int)mGameState.at(i).size(); j++)
        {
            if(mGameState.at(i).at(j))
            {
                delete mGameState.at(i).at(j);
            }
        }
    }
}

void GameBoard::update()
{
    // キーボードの状態を取得
    inputKeyboard();

    if(mIsUpdated)
    {
        pickTetromino();
        inputNPC();
        hold();
        updateActiveTetromino();
        updateGameState();
        updateScore();
        
        mUpdateFrame ++;
    }
}

void GameBoard::gameover()
{
    // フラグの設定
    mIsGameover = true;
    mIsUpdated = false;
    // テクスチャの変更
    for(int y = 0; y < GAMEBOARD_VERTICAL; y++)
    {
        for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
        {
            if(mGameState.at(y).at(x))
            {
                mGameState.at(y).at(x)->setTexture(mGame->getTexture("image/blocks/none.png"));
            }
        }
    }
}

void GameBoard::inputKeyboard()
{
    // keyboardの状態の更新
    mKeyboardState = mGame->getKeyboardState();
}

void GameBoard::inputNPC()
{
    // mActiveTetrominoの挙動に関わる操作を無効化
    mKeyboardState[SDL_SCANCODE_A] = 0;
    mKeyboardState[SDL_SCANCODE_D] = 0;
    mKeyboardState[SDL_SCANCODE_W] = 0;
    mKeyboardState[SDL_SCANCODE_S] = 0;
    mKeyboardState[SDL_SCANCODE_H] = 0;
    mKeyboardState[SDL_SCANCODE_J] = 0;
    mKeyboardState[SDL_SCANCODE_K] = 0; 

    // NPCの演算が終わっていない場合とmActiveTetrominoが存在しない場合何もしない
    if(NPC::isCalculating() || !mActiveTetrominio)
    {
        return;   
    }
    
    // mNPCResultの初期化
    if(!mIsInitializeResult)
    {
        mNPCResult = NPC::getResult();
        mIsInitializeResult = true;
    }

    // ホールドの処理
    if(mNPCResult.isHoled)
    {
        mKeyboardState[SDL_SCANCODE_H] = 1;
        mNPCResult.isHoled = false;
        return;
    }
    // 回転移動処理
    if(!mNPCResult.direction % 4 == 0)
    {
        mKeyboardState[SDL_SCANCODE_K] = 1;
        mNPCResult.direction -= 1;
        return;
    }
    // 並行移動処理
    if(mActiveTetrominio->getMoveFrame() >= mUpdateFrame - 1)
    {
        if(mNPCResult.coordinate < mActiveTetrominio->getCenter().x)
        {
            mKeyboardState[SDL_SCANCODE_A] = 1;
            return;
        }
        else if(mNPCResult.coordinate > mActiveTetrominio->getCenter().x)
        {
            mKeyboardState[SDL_SCANCODE_D] = 1;
            return;
        }
    }

    // クイックドロップ
    mKeyboardState[SDL_SCANCODE_W] = 1;
    mIsInitializeResult = false;
}

void GameBoard::pickTetromino()
{
    // mActiveTetrominoが存在する場合、何もせずに返す
    if(mActiveTetrominio)
    {
        return;
    }

    // mPendingTetrominoが空の場合、7種のミノのタイプを格納
    if(mPendingTetromino.empty())
    {
        mPendingTetromino.push_back(I);
        mPendingTetromino.push_back(O);
        mPendingTetromino.push_back(T);
        mPendingTetromino.push_back(L);
        mPendingTetromino.push_back(J);
        mPendingTetromino.push_back(S);
        mPendingTetromino.push_back(Z);
    }

    // mActiveTetrominoとmNextBoardの更新
    EType type = mNextBoard[0]->getType();
    mActiveTetrominio = new Tetromino(mGame, 50, this, type);

    for(int i = 0; i < NEXT_SIZE; i++)
    {
        mNextBoard[i]->setType(mNextBoard[i+1]->getType());
    }
    type = mPendingTetromino[rand() % mPendingTetromino.size()];
    mNextBoard[NEXT_SIZE - 1]->setType(type);

    auto iterator = std::find(mPendingTetromino.begin(),
                              mPendingTetromino.end(),
                              type);
    mPendingTetromino.erase(iterator);

    // NPCが存在し、計算をしていないなら計算開始
    if(!NPC::isCalculating() && !mIsHolded)
    {
        std::vector<EType> next;
        for(auto board : mNextBoard)
        {
            next.push_back(board->getType());
        }

        mNPCThread.detach();
        mNPCThread = std::thread(NPC::startCalculation,
                                 mActiveTetrominio->getType(),
                                 mHoldBoard->getType(),
                                 next,
                                 mGameState);

        // NPC::isCalculationがtrueになるまで待つ
        auto timeout = SDL_GetTicks() + 100;
        while(!NPC::isCalculating())
        {
            // 無限ループ回避
            if(SDL_TICKS_PASSED(SDL_GetTicks(), timeout))
            {
                SDL_Log("Loop exited because an infinite loop may have occurred: %s", __func__);
                break;
            }
        }
    }
}

void GameBoard::hold()
{
    if(mKeyboardState[SDL_SCANCODE_H] != 1 ||
       mIsHolded == true)
    {
        return ;
    }

    EType temp = mHoldBoard->getType();
    mHoldBoard->setType(mActiveTetrominio->getType());
    for(int i = 0; i < (int)mActiveTetrominio->getBlock().size(); i++)
    {
        delete mActiveTetrominio->getBlock()[i];
    }

    delete mActiveTetrominio;
    mActiveTetrominio = nullptr;

    mIsHolded = true;
    if(temp != NONE)
    {
        mActiveTetrominio = new Tetromino(mGame, 50, this, temp);
    }
    else
    {
        pickTetromino();
    }
}

void GameBoard::updateActiveTetromino()
{
    int parallel = 0, vertical = 0, rotation = 0;
    bool isQuickDrop = false;
    if((mKeyboardState[SDL_SCANCODE_A] == 1) ||
       (mKeyboardState[SDL_SCANCODE_A] >= KEEPED_FRAME && 
       (mKeyboardState[SDL_SCANCODE_A] - KEEPED_FRAME) % MOVE_FRAME == 0))
        parallel --;
    if((mKeyboardState[SDL_SCANCODE_D] == 1) ||
       (mKeyboardState[SDL_SCANCODE_D] >= KEEPED_FRAME && 
       (mKeyboardState[SDL_SCANCODE_D] - KEEPED_FRAME) % MOVE_FRAME == 0))
        parallel ++;
    if((mKeyboardState[SDL_SCANCODE_S] == 1) ||
       (mKeyboardState[SDL_SCANCODE_S] >= KEEPED_FRAME && 
       (mKeyboardState[SDL_SCANCODE_S] - KEEPED_FRAME) % MOVE_FRAME == 0))
        vertical --;
    if(mKeyboardState[SDL_SCANCODE_J] == 1)
        rotation --;
    if(mKeyboardState[SDL_SCANCODE_K] == 1)
        rotation ++;
    if(mKeyboardState[SDL_SCANCODE_W] == 1)
        isQuickDrop = true;
    
    
    mActiveTetrominio->parallelMove(parallel);
    mActiveTetrominio->verticalMove(vertical);
    mActiveTetrominio->rotationMove(rotation);
    mActiveTetrominio->quickDrop(isQuickDrop);
    mActiveTetrominio->updateShadow();
}

void GameBoard::updateGameState()
{
    if(mUpdateFrame - mActiveTetrominio->getMoveFrame() < FIX_COUNT && 
       !mActiveTetrominio->isQuickDrop())
    {
        return ;
    }

    // ブロックの固定
    Vector2 temp;
    for(auto block : mActiveTetrominio->getBlock())
    {
        temp = block->getCoordinate();
        mGameState[temp.y][temp.x] = block;
    }
    // mActiveTetrominoの削除
    delete mActiveTetrominio;
    mActiveTetrominio = nullptr;

    // ブロックで埋め尽くされた列の探索
    std::vector<int> filledLine;
    for(int y = 0; y < GAMEBOARD_VERTICAL; y++)
    {
        for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
        {
            if(!mGameState[y][x])
            {
                break;
            }
            else if(x == GAMEBOARD_PARALLEL - 1)
            {
                filledLine.push_back(y);
            }
        }
    }

    // mScoresの更新
    int score = atoi(mScores[1].c_str());
    int deletedLine = atoi(mScores[2].c_str());
    int usedMino = atoi(mScores[3].c_str()) + 1;
    int number;
    switch (filledLine.size())
    {
        case(1):
            deletedLine += 1;
            score += 200;
            number = atoi(mScores[7].c_str()) + 1;
            mScores[7] = std::to_string(number);
            break;
        case(2):
            deletedLine += 2;
            score += 400;
            number = atoi(mScores[6].c_str()) + 1;
            mScores[6] = std::to_string(number);
            break;
        case(3):
            deletedLine += 3;
            score += 800;
            number = atoi(mScores[5].c_str()) + 1;
            mScores[5] = std::to_string(number);
            break;
        case(4):
            deletedLine += 4;
            score += 1600;
            number = atoi(mScores[4].c_str()) + 1;
            mScores[4] = std::to_string(number);
            break;
        default:
            break;
    }
    mScores[1] = std::to_string(score);
    mScores[2] = std::to_string(deletedLine);
    mScores[3] = std::to_string(usedMino);

    // 埋め尽くされた列の削除と新しい列の作成
    while(!filledLine.empty())
    {
        // 埋め尽くされた列の中で一番下の列の削除
        for(auto block : mGameState[*filledLine.begin()])
        {
            delete block;
        }
        mGameState.erase(mGameState.begin() + *(filledLine.begin()));

        // 削除した列より上のブロックのy座標を1下げる
        for(int y = *filledLine.begin();
            y < (int)mGameState.size();
            y++)
        {
            for(auto block : mGameState[y])
            {
                if(block)
                {
                    temp = block->getCoordinate();
                    temp.y -= 1;
                    block->setCoordinate(temp);
                }
            }
        }

        // 新しい列の追加
        std::array<Block*, 10> line = {nullptr};
        mGameState.push_back(line);

        // filledLineの値を全て１ずつ減らし、使用した値の削除
        for(int i = 0; i < (int)filledLine.size(); i++)
        {
            filledLine[i] -= 1;
        }
        filledLine.erase(filledLine.begin());
    }
    
    mIsHolded = false;
    // mGameBoardに格納されているブロックの更新
    updateBlockPosition();
}

void GameBoard::updateBlockPosition()
{
    for(int y = 0; y < (int)mGameState.size(); y++)
    {
        for(int x = 0; x < (int)mGameState[y].size(); x++)
        {
            if(mGameState[y][x])
            {
                mGameState[y][x]->updatePosition();
            }
        }
    }
}

void GameBoard::updateScore()
{
    // それぞれの計算
    // IGT
    int hour, minute, second, millisecond, remainder;
    int frameCount = mUpdateFrame;
    hour = frameCount / (60 * 60 * 60);
    minute = (frameCount - hour * (60 * 60 * 60)) / (60 * 60);
    second = (frameCount - hour * (60 * 60 * 60) - minute * (60 * 60)) / 60;
    remainder = (frameCount - hour * (60 * 60 * 60) - minute * (60 * 60)) % 60;
    millisecond =  5.0f / 3.0f * remainder;
    std::string igt = std::to_string(hour) + ":";
    // 桁数固定用
    if(minute < 10)
    {
        std::string m = "0" + std::to_string(minute) + ":";
        igt += m;
    }
    else
    {
        std::string m = std::to_string(minute) + ":";
        igt += m;
    }
    if(second < 10)
    {
        std::string s = "0" + std::to_string(second) + ".";
        igt += s;
    }
    else
    {
        std::string s = std::to_string(second) + ".";
        igt += s;
    }
    if(millisecond < 10)
    {
        std::string ms = "0" + std::to_string(millisecond);
        igt += ms;
    }
    else
    {
        igt += std::to_string(millisecond);
    }
    mScores[0] = igt;
}

void GameBoard::initializeHoldBoard()
{
    // HoldBoardの設定
    mHoldBoard = new SideBoard(mGame, 40, this);
    Vector2 position = getPosition();
    position.x -= mTextureSize.x / 2 + 65;
    position.y -= mTextureSize.y / 2 - 100;
    mHoldBoard->setPosition(position);
    mHoldBoard->updateRectangle();   
}

void GameBoard::initializeNextBoard()
{
    Vector2 position(mPosition.x + mTextureSize.x / 2 + 65,
                     mTextureSize.y / 2 - 375);
    for(int i = 0; i < NEXT_SIZE; i++)
    {
        mNextBoard[i] = new SideBoard(mGame, 40, this);
        position.y += 120;
        mNextBoard[i]->setPosition(position);
        mNextBoard[i]->updateRectangle();
    }

    // nextの設定
    for(int i = 0; i < NEXT_SIZE; i++)
    {
        EType type = mPendingTetromino[rand() % mPendingTetromino.size()];
        mNextBoard[i]->setType(type);

        auto iterator = std::find(mPendingTetromino.begin(),
                                mPendingTetromino.end(),
                                type);
        mPendingTetromino.erase(iterator);
    }
}

void GameBoard::initializeScoreBoard()
{
    mScoreBoard = new ScoreBoard(mGame, 120, this);
    for(int i = 0; i < (int)mScores.size(); i++)
    {
        mScores[i] = "0";
    }
}