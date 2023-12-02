#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "globals.h"
#include "RGBA_Color.h"
#include "Rectangle.h"
#include "Piece.h"

class Window
{
public:
    Window(const std::string &WindowName, int Width = globals::window::WIDTH, int Height = globals::window::HEIGHT);
    ~Window();
    // getters
    int getWidth() const;
    int getHeight() const;
    // methods
    void updateWindow(int Width, int Height);

    void setDrawColor(RGBA_Color color) const;
    void clearRenderer() const;
    void presentRenderer() const;

    void drawRectangle(Rectangle rect, RGBA_Color color);
    void drawRectangle(int x, int y, int w, int h, RGBA_Color color);

    void drawPiece(Piece piece, Rectangle *dest);
    void drawPiece(PieceEnum pieceType, PieceColor pieceColor, Rectangle *dest);

    void drawAttackedSquareCircle(Rectangle *dest, bool emptySquare = true);

private:
    SDL_Window *__window;
    SDL_Renderer *__renderer;
    SDL_Texture *__pieces;
    SDL_Texture *__attackedEmptySquareCircle;
    SDL_Texture *__attackedOccupiedSquareCircle;

    int __width, __height;

    // Getters
    SDL_Window *__getWindow() const;
    SDL_Renderer *__getRenderer() const;
    SDL_Texture *__getPieceTexture() const;
    SDL_Texture *__getEmptySquareAttackedTexture() const;
    SDL_Texture *__getOccupiedSquareAttackedTexture() const;

    // Setters
    void __setWidth(int width);
    void __setHeight(int height);
    // Helpers for Textures
    SDL_Texture *__loadTexture(const std::string &Path);
    SDL_Texture *__loadPieces();
    SDL_Texture *__loadEmptySquareAttacked();
    SDL_Texture *__loadOccupiedSquareAttacked();
    void __destroyTexture(SDL_Texture *&Texture);
    void __destroyTextures();
};
