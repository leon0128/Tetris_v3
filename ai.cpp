#include "ai.hpp"

std::vector<std::array<bool, GAMEBOARD_PARALLEL>> AI::mVirtualGameState;
Actor::EType AI::mActiveTetromino = NONE;
Actor::EType AI::mHoldTetromino = NONE;
std::vector<Actor::EType> AI::mNextTetromino;
bool AI::mIsStarted = false;
bool AI::mIsCalculating = false;
struct AI::Result AI::mResult;

void AI::startCalculation(EType active, 
                          EType hold, 
                          std::vector<EType> next,
                          std::vector<std::array<Block*, GAMEBOARD_PARALLEL>> gameState)
{
    mIsCalculating = true;

    mActiveTetromino = active;
    mHoldTetromino = hold;
    mNextTetromino = next;

    mVirtualGameState.clear();
    // mVirtualGameStateの設定
    for(int y = 0; y < (int)gameState.size(); y++)
    {
        std::array<bool, GAMEBOARD_PARALLEL> line;
        mVirtualGameState.push_back(line);
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                mVirtualGameState[y][x] = true;
            }
            else
            {
                mVirtualGameState[y][x] = false;
            }
        }
    }
    // 計算開始
    mIsStarted = true;
}

void AI::calculate()
{
    // 無限ループ
    while(1)
    {
        // 新しいデータを受け取るまで待機
        while(!mIsStarted)
        {
        }
        mIsStarted = false;

        // 計算処理
        // 埋めない場所の設定
        // Vector2 leastCoordinate = getLeastHeight(mVirtualGameState);
        // 計算結果を格納する配列
        // [0]: hold, [1]: direction, [2]: coordinate, [3]: emptyNum, [4]: maxHeight
        std::vector<std::array<int, 5>> results;

        std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState;
        for(int d = 0; d < 4; d++)
        {
            for(int c = 0; c < 10; c++)
            {
                gameState = updateGameState(mVirtualGameState,
                                            mActiveTetromino,
                                            d,
                                            c);
                // printVirtualGameState(gameState);
                // if(!isFilledX(gameState,
                //               leastCoordinate.x,
                //               leastCoordinate.y));
                {
                    std::array<int, 5> result = {0,
                                                 d, 
                                                 c, 
                                                 getEmptyNumber(gameState),
                                                 getMostHeight(gameState)};
                    results.push_back(result);
                }
            }
        }
        if(mHoldTetromino == NONE)
        {
            mHoldTetromino = mNextTetromino[0];
        }
        for(int d = 0; d < 4; d++)
        {
            for(int c = 0; c < 10; c++)
            {
                gameState = updateGameState(mVirtualGameState,
                                            mHoldTetromino,
                                            d,
                                            c);
                // printVirtualGameState(gameState);
                // if(!isFilledX(gameState,
                //               leastCoordinate.x,
                //               leastCoordinate.y));
                {
                    std::array<int, 5> result = {1,
                                                 d, 
                                                 c, 
                                                 getEmptyNumber(gameState),
                                                 getMostHeight(gameState)};
                    results.push_back(result);
                }
            }
        }
        int emptyNumber = results[0][3];
        int maxHeight = 100;
        std::vector<int> resultIndex;
        for(auto result : results)
        {
            if(result[3] < emptyNumber)
            {
                emptyNumber = result[3];
            }
        }

        for(auto result : results)
        {
            if(result[4] < maxHeight &&
               result[3] == emptyNumber)
            {
                maxHeight = result[4];
            }
        }

        for(int i = 0; i < (int)results.size(); i++)
        {
            if(results[i][3] == emptyNumber &&
               results[i][4] == maxHeight)
            {
                resultIndex.push_back(i);
            }
        }

        std::array<int, 5> result = results[resultIndex[rand() % resultIndex.size()]];

        mResult.isHoled = result[0];
        mResult.direction = result[1];
        mResult.coordinate = result[2];

        SDL_Log("hold: %d, direction: %d, coordinate: %d, empty: %d, height: %d", result[0], result[1], result[2], result[3], result[4]);
        mIsCalculating = false;
    }
}

Vector2 AI::getLeastHeight(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState)
{
    int heights[GAMEBOARD_PARALLEL] = {-1};
    bool isEmpty = true;

    // 各x座標の最高点を取得
    for(int y = 0; y < (int)gameState.size(); y++)
    {
        isEmpty = true;
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                heights[x] = y;
                isEmpty = false;
            }
        }
        if(isEmpty)
        {
            break;
        }
    }

    // 最高点が最も低いものの中から無作為に選択肢戻す
    int min = GAMEBOARD_VERTICAL;
    for(auto height : heights)
    {
        if(min > height)
        {
            min = height;
        }
    }
    std::vector<int> mins;
    for(int i = 0; i < (int)gameState[0].size(); i++)
    {
        if(min == heights[i])
        {
            mins.push_back(i);
        }
    }
    
    Vector2 coordinate(mins[rand() % mins.size()], mins[0]);
    return coordinate;
}

std::vector<std::array<bool, GAMEBOARD_PARALLEL>> AI::updateGameState(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState,
                         EType type,
                         int direction,
                         int coordinate)
{
    auto tetromino = getInitializeTetrominoCoordinate(type);
    tetromino = getRotationTetrominoCoordinate(tetromino, direction);
    tetromino = getParallelTetrominoCoordinate(tetromino, coordinate);
    std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gs = getQuickDropedGameState(gameState, tetromino);
    return gs;
}

std::array<Vector2, 4> AI::getInitializeTetrominoCoordinate(EType type)
{
    std::array<Vector2, 4> tetromino;

    switch(type)
    {
        case(I):
            tetromino[0].set(4, 19);
            tetromino[1].set(5, 19);
            tetromino[2].set(3, 19);
            tetromino[3].set(6, 19);
            break;
        
        case(O):
            tetromino[0].set(4, 19);
            tetromino[1].set(4, 18);
            tetromino[2].set(5, 18);
            tetromino[3].set(5, 19);
            break;

        case(T):
            tetromino[0].set(4, 18);
            tetromino[1].set(4, 19);
            tetromino[2].set(3, 18);
            tetromino[3].set(5, 18);
            break;

        case(L):
            tetromino[0].set(4, 18);
            tetromino[1].set(5, 18);
            tetromino[2].set(3, 18);
            tetromino[3].set(5, 19);
            break;

        case(J):
            tetromino[0].set(4, 18);
            tetromino[1].set(3, 18);
            tetromino[2].set(5, 18);
            tetromino[3].set(3, 19);
            break;

        case(S):
            tetromino[0].set(4, 18);
            tetromino[1].set(4, 19);
            tetromino[2].set(3, 18);
            tetromino[3].set(5, 19);
            break;

        case(Z):
            tetromino[0].set(4, 18);
            tetromino[1].set(4, 19);
            tetromino[2].set(5, 18);
            tetromino[3].set(3, 19);
            break;
        
        default:
            break;
    }
    return tetromino;
}

std::array<Vector2, 4> AI::getRotationTetrominoCoordinate(std::array<Vector2, 4> tetromino,
                                                          int direction)
{
    Vector2 target, distance;
    for(int i = 0; i < direction % 4; i++)
    {
        for(int j = 0; j < (int)tetromino.size(); j++)
        {
            target = tetromino[j];
            distance = target - tetromino[0];

            target.x += distance.y - distance.x;
            target.y += (int)(-1) * distance.x - distance.y;

            tetromino[j] = target;
        }
    }
    return tetromino;
}

std::array<Vector2, 4> AI::getParallelTetrominoCoordinate(std::array<Vector2, 4> tetromino,
                                                      int coordinateX)
{
    bool isCorrect = true;
    while(tetromino[0].x != coordinateX && isCorrect)
    {
        
        if(coordinateX < tetromino[0].x)
        {
            for(int i = 0; i < (int)tetromino.size(); i++)
            {
                tetromino[i].x -= 1;
                if(tetromino[i].x < 0)
                {
                    isCorrect = false;
                }
            }
            if(!isCorrect)
            {
                for(int i = 0; i < (int)tetromino.size(); i++)
                {
                    tetromino[i].x += 1;
                }
            }
        }

        if(coordinateX > tetromino[0].x)
        {
            for(int i = 0; i < (int)tetromino.size(); i++)
            {
                tetromino[i].x += 1;
                if(tetromino[i].x > 9)
                {
                    isCorrect = false;
                }
            }
            if(!isCorrect)
            {
                for(int i = 0; i < (int)tetromino.size(); i++)
                {
                    tetromino[i].x -= 1;
                }
            }
        }
    }

    return tetromino;
}

std::vector<std::array<bool, GAMEBOARD_PARALLEL>> AI::getQuickDropedGameState(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState,
                                 std::array<Vector2, 4> tetromino)
{
    bool isCorrect = true;
    while(isCorrect)
    {
        for(int i = 0; i < (int)tetromino.size(); i++)
        {
            tetromino[i].y -= 1;
            if(tetromino[i].y < 0 ||
               gameState[tetromino[i].y][tetromino[i].x])
            {
                isCorrect = false;
            }
        }
        if(!isCorrect)
        {
            for(int i = 0; i < (int)tetromino.size(); i++)
            {
                tetromino[i].y += 1;
            }
        }
    }

    // テトロミノの固定
    for(int i = 0; i < (int)tetromino.size(); i++)
    {
        gameState[tetromino[i].y][tetromino[i].x] = true;
    }
    
    return gameState;
}

bool AI::isFilledX(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState,
                   int x,
                   int leastHeight)
{
    for(int y = leastHeight + 1; y < (int)gameState.size(); y++)
    {
        if(gameState[y][x])
        {
            return true;
        }
    }
    return false;
}

int AI::getMostHeight(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState)
{
    bool isEmpty = false;
    int height = 0;
    for(int y = 0; y < (int)gameState.size(); y++)
    {
        isEmpty = true;
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                height = y;
                isEmpty = false;
            }
        }
        if(isEmpty)
        {
            break;
        }
    }

    return height;
}

int AI::getEmptyNumber(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState)
{
    bool isEmpty[GAMEBOARD_PARALLEL] = {false};
    bool isEmptyLine = true;

    int number = 0;
    for(int y = 0; y < (int)gameState.size(); y++)
    {
        isEmptyLine = true;
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                isEmptyLine = false;
            }
            if(!gameState[y][x])
            {
                isEmpty[x] = true;
            }
            else if(isEmpty[x])
            {
                number ++;
            }
        }
        if(isEmptyLine)
        {
            break;
        }
    }
    return number;
}

void AI::printVirtualGameState(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gameState)
{
    for(int y = (int)gameState.size() - 1; y >= 0; y--)
    {
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                printf("||");
            }
            else
            {
                printf("--");
            }
        }
        printf(" :%d\n", y);
    }
    printf("0 1 2 3 4 5 6 7 8 9 \n");
}