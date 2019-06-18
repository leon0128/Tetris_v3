#pragma once

class Vector2
{
public:
    // メンバ変数
    int x;
    int y;
    
    // コンストラクタ
    Vector2():x(0), y(0){}
    explicit Vector2(int argX, int argY):x(argX), y(argY){}

    // メンバ変数の設定
    void set(int argX, int argY)
    {
        x = argX;
        y = argY;
    }
    void setX(int argX)
    {
        x = argX;
    }
    void setY(int argY)
    {   
        y = argY;
    }

    // 演算
    friend Vector2 operator+(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x + b.x, a.y + b.y);
    }
    friend Vector2 operator-(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x - b.x, a.y - b.y);
    }
    friend Vector2 operator*(const Vector2& vector, float scale)
    {
        return Vectorw(static_cast<int>(vector.x * scale), 
                       static_cast<int>(vector.y * scale));
    }
    friend Vector2 operator*(float scale, const Vector2& vector)
    {
        return Vectorw(static_cast<int>(vector.x * scale), 
                       static_cast<int>(vector.y * scale));     
    }
};