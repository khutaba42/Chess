#pragma once

// Forward declare SDL_Rect so no collisions will happen, include the header that contains SDL_Rect in the cpp unit.
struct SDL_Rect;

struct Rectangle
{
public:
    union
    {
        struct
        {
            int x, y;
            int w, h;
        };
        SDL_Rect Rect;
    };

public:
    Rectangle();
    Rectangle(int x, int y, int w, int h);
    // Rectangle(const SDL_Rect &rect);
    const Rectangle &operator=(const Rectangle &src);
    ~Rectangle();

    // if width or height is odd the taller side (by 1 pixel) will be to the right or bottom (for efficiency reasons)
    static Rectangle createRectangleFromMiddle(int MiddleX, int MiddleY, int Width, int Height);

    // Conversion operator to convert Rectangle to SDL_Rect
    operator SDL_Rect() const;

    friend bool operator==(const Rectangle &a, const Rectangle &b);
    friend bool operator!=(const Rectangle &a, const Rectangle &b);

    // use when you want to cast a pointer of type `SDL_Rect *` to `Rectangle *`
    friend SDL_Rect *cast_Rectangle_to_SDL_Rect(Rectangle *rectangle);
    // use when you want to cast a pointer of type `const SDL_Rect *` to `const Rectangle *`
    friend const SDL_Rect *cast_Rectangle_to_SDL_Rect(const Rectangle *rectangle);

    bool inRectangle(int x, int y) const;
};

#include <stdexcept>
#define CHECK_SDL_RECT_AND_RECTANGLE_DATA                                              \
    if (sizeof(SDL_Rect) != sizeof(Rectangle))                                         \
    {                                                                                  \
        throw std::logic_error("Rectangle and SDL_Rect are NOT the same size.");       \
    }                                                                                  \
    else                                                                               \
    {                                                                                  \
        SDL_Rect SDLrect;                                                              \
        SDLrect.x = 1;                                                                 \
        SDLrect.y = 2;                                                                 \
        SDLrect.w = 3;                                                                 \
        SDLrect.h = 4;                                                                 \
        Rectangle rect(1, 2, 3, 4);                                                    \
        if (SDLrect.x != rect.x ||                                                     \
            SDLrect.y != rect.y ||                                                     \
            SDLrect.w != rect.w ||                                                     \
            SDLrect.h != rect.h)                                                       \
        {                                                                              \
            throw std::logic_error("Rectangle and SDL_Rect are NOT the same Layout."); \
        }                                                                              \
    }
