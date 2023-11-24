#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "Window.h"
#include "Board.h"
#include "Piece.h"
#include "Rectangle.h"
class Graphics
{
public:
    Graphics();
    ~Graphics();

    GameStatus handleEvents();

private:
    Window __window;

    Board __board;
    struct Rect_data
    {
        Rectangle BoardFrame;
        int SquareSideLength;
        Rectangle BoardBorders;
        std::vector<Rectangle> Squares;
        std::pair<bool, Vec2<int>> IlluminatedSquare;

        Rect_data(const Graphics &graphics, const Board &board);
    };
    Rect_data __rectangles;
    SDL_Texture *__piecesTexture;

    //-----------------//-----------------//-----------------//-----------------//
    // Helper Functions
    int getWindowWidth() const;
    int getWindowHeight() const;
    SDL_Renderer *getRenderer();
    void updateWindow(int Width, int Height);

    void updateRectData();
    void drawBoard();

    void handleMousePosition(int x, int y);

    SDL_Texture *loadPieceTexture(const std::string &Path);
    void destroyTexture(SDL_Texture *&Texture);
};
