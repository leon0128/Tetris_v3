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

    // 結果の初期化
    mResult.isHoled = 1;
    mResult.direction = 2;
    mResult.coordinate = 6;

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
    // 詳細な結果を格納する構造体
    struct DetailResult
    {
        Result result;
        int empty;
        int maxHeight;
        double dispersion;
    };

    // 結果を格納する配列
    std::vector<DetailResult> detailResultVector;

    // ホールドと通常の合わせて80回
    for(int h = 0; h < 2; h++)
    {
        EType type = mActiveTetromino;
        if(h == 1)
        {
            if(mHoldTetromino != NONE)
            {
                type = mNextTetromino[0];
            }
            else
            {
                type = mHoldTetromino;
            }
        }

        for(unsigned int d = 0; d < 4; d++)
        {
            for(int c = 0; c < 10; c++)
            {
                VirtualGameState gameState = updateGameState(mVirtualGameState,
                                                             type,
                                                             d,
                                                             c);
                Result result = {d, c, h};
                DetailResult detail = {result,
                                       getEmptyNumber(gameState),
                                       getMaxHeight(gameState),
                                       getDispersion(gameState)};
                detailResultVector.push_back(detail);
            }
        }
    }

    // emptyが最小のもの以外の削除
    int minEmpty = std::numeric_limits<int>::max();
    for(auto detail : detailResultVector)
    {
        if(detail.empty < minEmpty)
        {
            minEmpty = detail.empty;
        }
    }
    auto iterator = detailResultVector.begin();
    while(iterator != detailResultVector.end())
    {
        if(iterator->empty != minEmpty)
        {
            std::iter_swap(iterator, detailResultVector.end() -1);
            detailResultVector.pop_back();
        }
        else
        {
            iterator++;
        }
    }

    // maxHeightが最小のもの
    int minHeight = std::numeric_limits<int>::max();
    for(auto detail : detailResultVector)
    {
        if(detail.maxHeight < minHeight)
        {
            minHeight = detail.maxHeight;
        }
    }
    iterator = detailResultVector.begin();
    while(iterator != detailResultVector.end())
    {
        if(iterator->maxHeight != minHeight)
        {
            std::iter_swap(iterator, detailResultVector.end() -1);
            detailResultVector.pop_back();
        }
        else
        {
            iterator++;
        }
    }

    // dispersionの最小値のみを残す
    double minDispersion = std::numeric_limits<double>::max();
    for(auto detail : detailResultVector)
    {
        if(detail.dispersion < minDispersion)
        {
            minDispersion = detail.dispersion;
        }
    }
    iterator = detailResultVector.begin();
    while(iterator != detailResultVector.end())
    {
        if(iterator->dispersion != minDispersion)
        {
            std::iter_swap(iterator, detailResultVector.end() -1);
            detailResultVector.pop_back();
        }
        else
        {
            iterator++;
        }
    }

    DetailResult detailResult = detailResultVector.at(rand() % detailResultVector.size());
    // mResult = detailResult.result;
    SDL_Log("empty: %d, dispersion: %lf", detailResult.empty, detailResult.dispersion);
    sleep(5);
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
    // 各x軸で一番高い位置にあるブロックのy座標を格納
    // ブロックが存在しない場合は、-1となる
    std::array<int, GAMEBOARD_PARALLEL> maxHeights;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = GAMEBOARD_VERTICAL -1; y >= 0; y--)
        {
            if(gameState.at(y).at(x))
            {
                maxHeights.at(x) = y;
                break;
            }
            else if(y == 0)
            {
                maxHeights.at(x) = -1;
            }
        }
    }
    
    // 各x座標の最大値より下のブロックの空白の数を調査
    int emptyNumber = 0;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = 0; y < maxHeights.at(x); y++)
        {
            if(!gameState.at(y).at(x))
            {
                emptyNumber++;
            }
        }
    }
    
    return emptyNumber;
}

double NPC::getDispersion(VirtualGameState gameState)
{
    // 各x軸で一番高い位置にあるブロックのy座標を格納
    // ブロックが存在しない場合は、-1となる
    std::array<int, GAMEBOARD_PARALLEL> maxHeights;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = GAMEBOARD_VERTICAL -1; y >= 0; y--)
        {
            if(gameState.at(y).at(x))
            {
                maxHeights.at(x) = y + 1;
                break;
            }
            else if(y == 0)
            {
                maxHeights.at(x) = 0;
            }
        }
    }

    // 高さの分散を求める
    double dispersion = 0.0, average = 0.0, sum = 0;
    // 平均
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        sum += maxHeights.at(x);
    }
    average = sum / GAMEBOARD_PARALLEL;
    // 分散
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        dispersion = dispersion +
                     (maxHeights.at(x) - average) * 
                     (maxHeights.at(x) - average);
    }
    if(average != 0.0)
    {
        dispersion = dispersion / average;
    }
    else
    {
        dispersion = 0.0;
    }
    
    return dispersion;
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