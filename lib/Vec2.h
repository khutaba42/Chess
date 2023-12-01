#pragma once

template <typename T>
struct Vec2
{
    union
    {
        T x;
        T row;
    };
    union
    {
        T y;
        T col;
    };

    Vec2() {} // garbage values (no initialization for speed)
    Vec2(T x, T y) : x(x), y(y) {}
    friend bool operator==(const Vec2 a, const Vec2 b) { return (a.x == b.x) && (a.y == b.y); }
    friend bool operator!=(const Vec2 a, const Vec2 b) { return !(a == b); }
    friend Vec2 operator+(const Vec2 a, const Vec2 b) { return Vec2(a.x + b.x, a.y + b.y); }
    Vec2 &operator+=(const Vec2 other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    friend Vec2 operator-(const Vec2 a, const Vec2 b) { return Vec2(a.x - b.x, a.y - b.y); }
    Vec2 &operator-=(const Vec2 other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }
};
