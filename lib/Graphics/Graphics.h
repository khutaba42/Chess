#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <memory>

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
        struct ClickedPiece
        {
            bool Clicked;
            Vec2<int> Position;
            Rectangle CurrentRectanglePosition;

            ClickedPiece(bool Clicked = false, Vec2<int> position = {-1, -1}, Rectangle CurrentRectanglePosition = {0, 0, 0, 0}) : Clicked(Clicked), Position(position), CurrentRectanglePosition(CurrentRectanglePosition) {}
        };
        std::unique_ptr<ClickedPiece> ClickedPieceInfo;

        Rect_data(const Graphics &graphics, const Board &board);
    };
    Rect_data __rectangles;
    SDL_Texture *__piecesTexture;

    //-----------------//-----------------//-----------------//-----------------//
    // Helper Functions
    int __getWindowWidth() const;
    int __getWindowHeight() const;
    SDL_Renderer *__getRenderer();
    void __updateWindow(int Width, int Height);

    void __updateRectData();
    void __drawBoard();

    Vec2<int> __getSquare(int x, int y);
    Vec2<int> __getSquare(Vec2<int> Coordinate);

    void __clickOnPiece(int x, int y);
    void __releasePiece(int x, int y);
    void __updateClickedPieceRectanglePosition(int x, int y);

    void __handleMousePosition(int x, int y);
    void __handleMouseDownEvent(int x, int y);
    void __handleMouseUpEvent(int x, int y);

    SDL_Texture *__loadPieceTexture(const std::string &Path);
    void __destroyTexture(SDL_Texture *&Texture);
};
