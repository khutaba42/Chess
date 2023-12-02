#include <SDL2/SDL.h>

#include <utility> // for std::pair
#include <map>
#include <string>

#include "Window.h"

#include "globals.h"
#include "RGBA_Color.h"
#include "Rectangle.h"
#include "Piece.h"

Window::Window(const std::string &WindowName, int Width, int Height)
    : __window(nullptr),
      __renderer(nullptr),
      __pieces(nullptr),
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

    // load textures
    __pieces = __loadPieces();
    __attackedEmptySquareCircle = __loadEmptySquareAttacked();
    __attackedOccupiedSquareCircle = __loadOccupiedSquareAttacked();
}

Window::~Window()
{
    // Cleanup and exit
    __destroyTextures();
    SDL_DestroyRenderer(__renderer);
    SDL_DestroyWindow(__window);
    // Quit SDL subsystems
    SDL_Quit();
}

int Window::getWidth() const { return __width; }
int Window::getHeight() const { return __height; }

void Window::updateWindow(int Width, int Height)
{
    __setWidth(Width);
    __setHeight(Height);
}

void Window::setDrawColor(RGBA_Color color) const
{
    SDL_SetRenderDrawColor(
        __getRenderer(),
        color.red(),
        color.green(),
        color.blue(),
        color.opacity()

    );
}

void Window::clearRenderer() const
{
    SDL_RenderClear(__getRenderer());
}

void Window::presentRenderer() const
{
    SDL_RenderPresent(__getRenderer());
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

const int SPRITE_PIECE_SIDE_LENGTH_VER_1 = 133;
std::map<std::pair<PieceEnum, PieceColor>, Rectangle> pieceRectanglesVER1 = {
    // Pieces
    {   {PieceEnum::King    , PieceColor::Black },  {0  , 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Queen   , PieceColor::Black },  {133, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Bishop  , PieceColor::Black },  {266, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Knight  , PieceColor::Black },  {399, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Rook    , PieceColor::Black },  {532, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Pawn    , PieceColor::Black },  {665, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::King    , PieceColor::White },  {0  , 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Queen   , PieceColor::White },  {133, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Bishop  , PieceColor::White },  {266, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Knight  , PieceColor::White },  {399, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Rook    , PieceColor::White },  {532, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Pawn    , PieceColor::White },  {665, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},

    // None
    {{PieceEnum::None, PieceColor::None}, {0, 0, 0, 0}}

};
const int SPRITE_PIECE_SIDE_LENGTH_VER_2 = 64;
std::map<std::pair<PieceEnum, PieceColor>, Rectangle> pieceRectanglesVER2 = {
    // Pieces
    {   {PieceEnum::King    , PieceColor::Black },  {0  , 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Queen   , PieceColor::Black },  {64 , 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Rook    , PieceColor::Black },  {128, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Knight  , PieceColor::Black },  {192, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Bishop  , PieceColor::Black },  {256, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Pawn    , PieceColor::Black },  {320, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::King    , PieceColor::White },  {0  , 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Queen   , PieceColor::White },  {64 , 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Rook    , PieceColor::White },  {128, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Knight  , PieceColor::White },  {192, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Bishop  , PieceColor::White },  {256, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Pawn    , PieceColor::White },  {320, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},

    // None
    {{PieceEnum::None, PieceColor::None}, {0, 0, 0, 0}}

};
const int SPRITE_PIECE_SIDE_LENGTH_VER_3 = 16;
std::map<std::pair<PieceEnum, PieceColor>, Rectangle> pieceRectanglesVER3 = {
    // Pieces
    {   {PieceEnum::King    , PieceColor::Black },  {0 , 16, SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Queen   , PieceColor::Black },  {16, 16, SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Rook    , PieceColor::Black },  {32, 16, SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Knight  , PieceColor::Black },  {48, 16, SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Bishop  , PieceColor::Black },  {64, 16, SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Pawn    , PieceColor::Black },  {80, 16, SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::King    , PieceColor::White },  {0 , 0 , SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Queen   , PieceColor::White },  {16, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Rook    , PieceColor::White },  {32, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Knight  , PieceColor::White },  {48, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Bishop  , PieceColor::White },  {64, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},
    {   {PieceEnum::Pawn    , PieceColor::White },  {80, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_3, SPRITE_PIECE_SIDE_LENGTH_VER_3}},

    // None
    {{PieceEnum::None, PieceColor::None}, {0, 0, 0, 0}}

};
std::map<std::pair<PieceEnum, PieceColor>, Rectangle> PieceRectangle = pieceRectanglesVER2;

void Window::drawPiece(Piece piece, Rectangle *dest)
{
    SDL_RenderCopy(__getRenderer(), __getPieceTexture(), cast_Rectangle_to_SDL_Rect(&PieceRectangle[{piece.type(), piece.color()}]), cast_Rectangle_to_SDL_Rect(dest));
}

void Window::drawPiece(PieceEnum pieceType, PieceColor pieceColor, Rectangle *dest)

{
    SDL_RenderCopy(__getRenderer(), __getPieceTexture(), cast_Rectangle_to_SDL_Rect(&PieceRectangle[{pieceType, pieceColor}]), cast_Rectangle_to_SDL_Rect(dest));
}

void Window::drawAttackedSquareCircle(Rectangle *dest, bool emptySquare)
{
    // srcrect – NULL for the entire texture
    SDL_RenderCopy(__getRenderer(), (emptySquare) ? __getEmptySquareAttackedTexture() : __getOccupiedSquareAttackedTexture(), nullptr, cast_Rectangle_to_SDL_Rect(dest));
}

SDL_Window *Window::__getWindow() const { return __window; }
SDL_Renderer *Window::__getRenderer() const { return __renderer; }
SDL_Texture *Window::__getPieceTexture() const { return __pieces; }
SDL_Texture *Window::__getEmptySquareAttackedTexture() const { return __attackedEmptySquareCircle; }
SDL_Texture *Window::__getOccupiedSquareAttackedTexture() const { return __attackedOccupiedSquareCircle; }

void Window::__setWidth(int width) { __width = width; }
void Window::__setHeight(int height) { __height = height; }

SDL_Texture *Window::__loadTexture(const std::string &Path)
{
    SDL_Surface *surface = nullptr;
    surface = SDL_LoadBMP(Path.c_str());
    if (!surface)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to create surface for image " + Path + "\n");
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(__getRenderer(), surface);
    if (!texture)
    {
        printf("Unable to load bitmap texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        throw std::runtime_error("Failed to create texture.\n");
    }
    SDL_FreeSurface(surface);

    return texture;
}

SDL_Texture *Window::__loadPieces()
{
    return __loadTexture(globals::piece::spritePath);
}
SDL_Texture *Window::__loadEmptySquareAttacked()
{
    return __loadTexture(globals::board::EMPTY_SQUARE_ATTACKED_PATH);
}

SDL_Texture *Window::__loadOccupiedSquareAttacked()
{
    return __loadTexture(globals::board::OCCUPIED_SQUARE_ATTACKED_PATH);
}

void Window::__destroyTexture(SDL_Texture *&Texture)
{
    // Passing NULL or an otherwise invalid texture to `SDL_DestroyTexture` will set the SDL error message to "Invalid texture".
    if (Texture)
    {
        SDL_DestroyTexture(Texture);
    }
    Texture = nullptr;
}

void Window::__destroyTextures()
{
    __destroyTexture(__pieces);
    __destroyTexture(__attackedEmptySquareCircle);
    __destroyTexture(__attackedOccupiedSquareCircle);
}
