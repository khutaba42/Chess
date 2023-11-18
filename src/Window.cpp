#include "Window.h"
#include "globals.h"

Window::Window(const std::string &WindowName)
    : __window(nullptr),
      __renderer(nullptr),
      __width(globals::window::WIDTH),
      __height(globals::window::HEIGHT)
{
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        throw std::runtime_error("Unable to initialize SDL.");
    }

    // Set OpenGL version (modify as needed)
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create a window
    __window = SDL_CreateWindow(WindowName.c_str(),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                globals::window::WIDTH,
                                globals::window::HEIGHT,
                                /*SDL_WINDOW_OPENGL |*/ SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

    // Create an OpenGL context
    // __glContext = SDL_GL_CreateContext(__window);
    // if (!__glContext)
    // {
    // SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
    // SDL_DestroyWindow(__window);
    // SDL_Quit();
    // throw std::runtime_error("Failed to create OpenGL context.");
    // }
}

Window::~Window()
{
    // Destroy window
    // SDL_GL_DeleteContext(__glContext);
    // Cleanup and exit
    SDL_DestroyRenderer(__renderer);
    SDL_DestroyWindow(__window);
    // Quit SDL subsystems
    SDL_Quit();
}

SDL_Window *Window::getWindow() const { return __window; }
SDL_Renderer *Window::getRenderer() const { return __renderer; }
int Window::getWidth() const { return __width; }
int Window::getHeight() const { return __height; }
void Window::setWidth(int width) { __width = width; }
void Window::setHeight(int height) { __height = height; }

void Window::updateWindow()
{
}

void Window::drawRectangle(int x, int y, int w, int h, Color color)
{
    // Specify the rectangle's position and dimensions
    SDL_Rect rect;
    rect.x = x; // X-coordinate of the top-left corner
    rect.y = y; // Y-coordinate of the top-left corner
    rect.w = w; // Width of the rectangle
    rect.h = h; // Height of the rectangle

    // Set the drawing color
    SDL_SetRenderDrawColor(__renderer, color.red(), color.green(), color.blue(), color.opacity());

    // Draw the filled rectangle
    SDL_RenderFillRect(__renderer, &rect);
}
