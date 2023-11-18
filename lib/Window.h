#pragma once

#include "includes.h"
#include "Color.h"

class Window
{
public:
    Window(const std::string &name);
    ~Window();

    SDL_Window *getWindow() const;
    SDL_Renderer *getRenderer() const;
    int getWidth() const;
    int getHeight() const;
    void setWidth(int width);
    void setHeight(int height);

    void updateWindow();

    void drawRectangle(int x, int y, int w, int h, Color color);

private:
    SDL_Window *__window = nullptr;
    SDL_Renderer *__renderer = nullptr;

    int __width, __height;

    // SDL_GLContext __glContext = nullptr;
};
