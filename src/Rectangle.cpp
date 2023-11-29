#include <SDL2/SDL.h>
#include "Rectangle.h"

Rectangle::Rectangle() {}

Rectangle::Rectangle(int x, int y, int w, int h)
    : x(x),
      y(y),
      w(w),
      h(h)
{
}

const Rectangle &Rectangle::operator=(const Rectangle &src)
{
    // To avoid self assignment bug
    if (&src != this)
    {
        this->x = src.x;
        this->y = src.y;
        this->w = src.w;
        this->h = src.h;
    }
    return *this;
}

Rectangle::~Rectangle()
{
}

Rectangle Rectangle::createRectangleFromMiddle(int MiddleX, int MiddleY, int Width, int Height)
{
    return Rectangle(
        MiddleX - (Width / 2),
        MiddleY - (Height / 2),
        Width,
        Height

    );
}

Rectangle::operator SDL_Rect() const
{
    return {x, y, w, h};
}

bool operator==(const Rectangle &a, const Rectangle &b)
{
    return a.x == b.x &&
           a.y == b.y &&
           a.w == b.w &&
           a.h == b.h;
}

bool operator!=(const Rectangle &a, const Rectangle &b)
{
    return !(a == b);
}

const SDL_Rect *cast_Rectangle_to_SDL_Rect(const Rectangle *rectangle)
{
    return (rectangle) ? (&(rectangle->Rect)) : (nullptr);
}

SDL_Rect *cast_Rectangle_to_SDL_Rect(Rectangle *rectangle)
{
    return (rectangle) ? (&(rectangle->Rect)) : (nullptr);
}

bool Rectangle::inRectangle(int x, int y) const
{
    return (x >= this->x) &&
           (y >= this->y) &&
           (x <= this->x + this->w) &&
           (y <= this->y + this->h);
}
