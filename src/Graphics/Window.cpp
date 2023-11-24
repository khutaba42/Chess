#include "Window.h"
#include "globals.h"
#include "Rectangle.h"

Window::Window(const std::string &WindowName, int Width, int Height)
    : __window(nullptr),
      __renderer(nullptr),
      __width(Width),
      __height(Height)
{
    // Checks
    CHECK_SDL_RECT_AND_RECTANGLE_DATA

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) // TODO: more efficient than SDL_INIT_EVERYTHING ?
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        throw std::runtime_error("Unable to initialize SDL.");
    }

    // Create a window
    __window = SDL_CreateWindow(
        WindowName.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        Width,
        Height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE

    );
    if (!__window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        throw std::runtime_error("Failed to create window.");
    }

    // Create a renderer
    __renderer = SDL_CreateRenderer(__window, -1, SDL_RENDERER_ACCELERATED);
    if (!__renderer)
    {
        // Handle renderer creation error
        SDL_DestroyWindow(__window);
        SDL_Quit();
        throw std::runtime_error("Failed to create renderer.");
    }
}

Window::~Window()
{
    // Cleanup and exit
    SDL_DestroyRenderer(__renderer);
    SDL_DestroyWindow(__window);
    // Quit SDL subsystems
    SDL_Quit();
}

void Window::updateWindow(int Width, int Height)
{
    setWidth(Width);
    setHeight(Height);
}

void Window::drawRectangle(Rectangle rect, RGBA_Color color)
{
    // Set the drawing color
    SDL_SetRenderDrawColor(__renderer, color.red(), color.green(), color.blue(), color.opacity());
    // Draw the filled rectangle
    SDL_RenderFillRect(__renderer, reinterpret_cast<SDL_Rect *>(&rect));
}

void Window::drawRectangle(int x, int y, int w, int h, RGBA_Color color)
{
    drawRectangle({x, y, w, h}, color);
}

void Window::drawTexture(SDL_Texture *Texture, Rectangle *src, Rectangle *dest)
{
    SDL_RenderCopy(this->getRenderer(), Texture, cast_Rectangle_to_SDL_Rect(src), cast_Rectangle_to_SDL_Rect(dest));
}

SDL_Window *Window::getWindow() const { return __window; }
SDL_Renderer *Window::getRenderer() const { return __renderer; }
int Window::getWidth() const { return __width; }
int Window::getHeight() const { return __height; }

void Window::setWidth(int width) { __width = width; }
void Window::setHeight(int height) { __height = height; }
