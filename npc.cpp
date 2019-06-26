#include "npc.hpp"

// 静的メンバ変数の初期化
NPC::VirtualGameState NPC::mVirtualGameState;
Actor::EType NPC::mActiveTetromino = NONE;
Actor::EType NPC::mHoldTetromino = NONE;
std::vector<Actor::EType> NPC::mNextTetromino;
bool NPC::mIsCalculating = false;
struct NPC::Result NPC::mResult;

void NPC::startCalculation(EType active, 
                          EType hold, 
                          std::vector<EType> next,
                          std::vector<std::array<Block*, GAMEBOARD_PARALLEL>> gameState)
{
    // 計算開始フラグ
    mIsCalculating = true;

    // 各メンバ変数の設定
    mActiveTetromino = active;
    mHoldTetromino = hold;
    mNextTetromino = next;

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

    // 計算結果をmResultに格納
    calculate();
    // mVirticalGameStateの削除
    mVirtualGameState.clear();

    // 計算終了フラグ
    mIsCalculating = false;
}

void NPC::calculate()
{
    // // 計算処理
    // // 計算結果を格納する配列
    // // [0]: hold, [1]: direction, [2]: coordinate, [3]: emptyNum, [4]: maxHeight, [5]: minHeight
    // std::vector<std::array<int, 6>> results;

    // VirtualGameState gameState;
    // for(int d = 0; d < 4; d++)
    // {
    //     for(int c = 0; c < 10; c++)
    //     {
    //         gameState = updateGameState(mVirtualGameState,
    //                                     mActiveTetromino,
    //                                     d,
    //                                     c);
    //         {
    //             std::array<int, 6> result = {0,
    //                                             d, 
    //                                             c, 
    //                                             getEmptyNumber(gameState),
    //                                             getMaxHeight(gameState),
    //                                             getMinHeight(gameState).y};
    //             results.push_back(result);
    //         }
    //     }
    // }
    // if(mHoldTetromino == NONE)
    // {
    //     mHoldTetromino = mNextTetromino[0];
    // }
    // for(int d = 0; d < 4; d++)
    // {
    //     for(int c = 0; c < 10; c++)
    //     {
    //         gameState = updateGameState(mVirtualGameState,
    //                                     mHoldTetromino,
    //                                     d,
    //                                     c);
    //         {
    //             std::array<int, 6> result = {1,
    //                                             d, 
    //                                             c, 
    //                                             getEmptyNumber(gameState),
    //                                             getMaxHeight(gameState),
    //                                             getMinHeight(gameState).y};
    //             results.push_back(result);
    //         }
    //     }
    // }
    // int emptyNumber = results[0][3];
    // int maxHeight = 100;
    // int minHeight = -1;
    // std::vector<int> resultIndex;
    // std::vector<int> newResultIndex;
    // for(auto result : results)
    // {
    //     if(result[3] < emptyNumber)
    //     {
    //         emptyNumber = result[3];
    //     }
    // }

    // for(auto result : results)
    // {
    //     if(result[4] < maxHeight &&
    //         result[3] == emptyNumber)
    //     {
    //         maxHeight = result[4];
    //     }
    // }

    // for(int i = 0; i < (int)results.size(); i++)
    // {
    //     if(results[i][3] == emptyNumber &&
    //         results[i][4] == maxHeight)
    //     {
    //         resultIndex.push_back(i);
    //     }
    // }

    // for(auto index : resultIndex)
    // {
    //     if(results[index][5] > minHeight)
    //     {
    //         minHeight = results[index][5];
    //     }
    // }

    // for(int i = 0; i < (int)resultIndex.size(); i++)
    // {
    //     if(results[resultIndex[i]][5] == minHeight)
    //     {
    //         newResultIndex.push_back(resultIndex[i]);
    //     }
    // }
    // std::array<int, 6> result = results[newResultIndex[rand() % newResultIndex.size()]];
    // SDL_Log("results: %d, narrow0: %d, narrow1: %d", (int)results.size(), (int)resultIndex.size(), (int)newResultIndex.size());
    // SDL_Log("hold: %d, direction: %d, coordinate: %d, empty: %d, maxHeight: %d, minHeight: %d", result[0], result[1], result[2], result[3], result[4], result[5]);
    // mResult.isHoled = result[0];
    // mResult.direction = result[1];
    // mResult.coordinate = result[2];

    // 深さ
    int depth = 2, index = 0;
    // 結果を格納する配列
    std::vector<std::array<int, 6>> resultVector;
    std::vector<int> resultIndex;
    
    // resultVectorに初期値の代入
    for(int isHold = 0; isHold < 2; isHold++)
    {
        for(int direction = 0; direction < 4; direction++)
        {
            for(int coordinate = 0; coordinate < 10; coordinate++)
            {
                std::array<int, 6> temp = {isHold,
                                           direction,
                                           coordinate,
                                           0,
                                           0,
                                           0};
                resultVector.push_back(temp);
                resultIndex.push_back(index);
                index ++;
            }
        }
    }

    // 何手先まで評価するか
    std::vector<VirtualGameState> gameStateVector = {mVirtualGameState};
    for(int d = 0; d < depth - 1; d++)
    {
        // resultVectorの中のみ評価
        for(auto index : resultIndex)
        {
            // typeの設定
            EType type = mActiveTetromino;
            if(resultVector[index][0] == 1)
            {
                type = mNextTetromino[0];
            }

            // 評価した結果を格納 
            gameStateVector[index] = updateGameState(gameStateVector[index],
                                                     type,
                                                     resultVector[index][1],
                                                     resultVector[index][2]);
            resultVector[index][3] = getEmptyNumber(gameStateVector[index]);
            resultVector[index][4] = getMaxHeight(gameStateVector[index]);
            resultVector[index][5] = getMinHeight(gameStateVector[index]);
        }
        
        // emptyの最小値の調査
        int empty = 100;
        for(auto index : resultIndex)
        {
            if(empty > resultVector[index][3])
            {
                empty = resultVector[index][3];
            }
        }
        // emptyの最小値以外のindexを削除
        auto iterator = resultIndex.begin();
        while(iterator != resultIndex.end())
        {
            if(*iterator != empty)
            {
                iterator = resultIndex.erase(iterator);
            }
            else
            {
                iterator++;
            }
        }

        // 最も高い位置にあるブロックのy座標を取得
        int max = -1;
        for(auto index : resultIndex)
        {
            if(max < resultVector[index][4])
            {
                max = resultVector[index][4];
            }
        }
        // 最大値以外のインデックスを削除
        iterator = resultIndex.begin();
        while(iterator != resultIndex.end())
        {
            if(*iterator != max)
            {
                iterator = resultIndex.erase(iterator);
            }
            else
            {
                iterator++;
            }
        }

    }
}

Vector2 NPC::getMinHeight(VirtualGameState gameState)
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

NPC::VirtualGameState NPC::updateGameState(VirtualGameState gameState,
                         EType type,
                         int direction,
                         int coordinate)
{
    auto tetromino = getInitializeTetrominoCoordinate(type);
    tetromino = getRotationTetrominoCoordinate(tetromino, direction);
    tetromino = getParallelTetrominoCoordinate(tetromino, coordinate);
    std::vector<std::array<bool, GAMEBOARD_PARALLEL>> gs = getQuickDropedGameState(gameState, tetromino);
    gs = deleteLine(gs);
    return gs;
}

std::array<Vector2, 4> NPC::getInitializeTetrominoCoordinate(EType type)
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

std::array<Vector2, 4> NPC::getRotationTetrominoCoordinate(std::array<Vector2, 4> tetromino,
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

std::array<Vector2, 4> NPC::getParallelTetrominoCoordinate(std::array<Vector2, 4> tetromino,
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

NPC::VirtualGameState NPC::getQuickDropedGameState(VirtualGameState gameState,
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

NPC::VirtualGameState NPC::deleteLine(VirtualGameState gameState)
{
    std::vector<int> filledLine;
    for(int y = 0; y < GAMEBOARD_VERTICAL; y++)
    {
        for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
        {
            if(!gameState[y][x])
            {
                break;
            }
            else if(x == GAMEBOARD_PARALLEL - 1)
            {
                filledLine.push_back(y);
            }
        }
    }

    while(!filledLine.empty())
    {
        gameState.erase(gameState.begin() + *(filledLine.begin()));
        std::array<bool, GAMEBOARD_PARALLEL> line = {false};
        gameState.push_back(line);
        for(int i = 0; i < (int)filledLine.size(); i++)
        {
            filledLine[i] -= 1;
        }
        filledLine.erase(filledLine.begin());
    }

    return gameState;
}

bool NPC::isFilledX(VirtualGameState gameState,
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

int NPC::getMaxHeight(VirtualGameState gameState)
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

int NPC::getEmptyNumber(VirtualGameState gameState)
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

void NPC::printVirtualGameState(VirtualGameState gameState)
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