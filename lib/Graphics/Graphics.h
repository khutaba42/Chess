#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "Window.h"
#include "Board.h"
#include "Rectangle.h"
#include "Vec2.h"

class Graphics
{
public:
    Graphics();
    ~Graphics();

    GameStatus handleEvents();

private:
    Window __window;

    Board __board;
    struct GraphicalBoard
    {
        Rectangle boardFrame;
        int squareSideLength;
        Rectangle boardBorders;
        struct SquareData
        {
            Rectangle coordinates;
            Vec2<int> position;
            bool attacked;

            SquareData() {}
            SquareData(Rectangle coords, Vec2<int> pos) : coordinates(coords), position(pos), attacked(false) {}
        };
        std::vector<SquareData> squareData;
        struct MouseInfo
        {
            bool hovering;
            Vec2<int> hoveredOnPosition;
            bool clicked;
            Vec2<int> clickedPieceInitalPosition;
            Rectangle clickedPieceRectangle;
        };
        MouseInfo mouseInfo;
    };
    GraphicalBoard __boardData;

    //-----------------//-----------------//-----------------//-----------------//
    // Getters
    // might throw and std::out_of_range error
    GraphicalBoard &__getGraphicalBoardData();
    GraphicalBoard::SquareData &__getSquareDataAt(Vec2<int> Pos);
    GraphicalBoard::SquareData &__getSquareDataAt(int row, int col);
    int __getWindowWidth() const;
    int __getWindowHeight() const;

    // Helper
    void __renderGame();
    // Helper Functions for Window
    void __updateWindow(int Width, int Height);

    // Helper Functions for Board Graphics
    void __updateGraphicalBoardData();
    void __drawBoard();

    bool __pieceIsClicked(Vec2<int> Position);

    void __handleMousePosition(int x, int y);
    void __handleMouseDownEvent(int x, int y);
    void __handleMouseUpEvent(int x, int y);

    void __clickOnPiece(int x, int y);
    void __releasePiece(int x, int y);
    void __updateClickedPieceRectanglePosition(int x, int y);
};
