#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "globals.h"
#include "RGBA_Color.h"
#include "Rectangle.h"

class Window
{
public:
    Window(const std::string &WindowName, int Width = globals::window::WIDTH, int Height = globals::window::HEIGHT);
    ~Window();
    // methods
    void updateWindow(int Width, int Height);
    void drawRectangle(Rectangle rect, RGBA_Color color);
    void drawRectangle(int x, int y, int w, int h, RGBA_Color color);
    void drawTexture(SDL_Texture *Texture, Rectangle*src, Rectangle*dest);
    // getters
    SDL_Window *getWindow() const;
    SDL_Renderer *getRenderer() const;
    int getWidth() const;
    int getHeight() const;

private:
    SDL_Window *__window;
    SDL_Renderer *__renderer;
    int __width, __height;

    // setters
    void setWidth(int width);
    void setHeight(int height);
};
