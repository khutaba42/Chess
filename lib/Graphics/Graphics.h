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
    // this struct holds in all the info needed to render the board on the window
    struct GraphicalBoard
    {
        // if false the white pieces will be on the bottom of the board 
        bool BoardIsFlipped = false; 

        // the board's main dimensions
        Rectangle boardFrame;
        int squareSideLength;
        Rectangle boardBorders;

        // this struct will hold in all the info needed for each square
        struct SquareData
        {
            Vec2<int> position; // the position in the board it represents (this solves the issue when flipping the board)
            Rectangle coordinates; // the actual coordinates of the rectangle (square) on the window

            SquareData() {}
            SquareData(Rectangle coords, Vec2<int> pos) : coordinates(coords), position(pos) {}
        };
        std::vector<SquareData> squareData; // the grid of square data

        // this struct hold in the info for mouse input on the squares
        struct MouseInfo
        {
            Validity<Vec2<int>> hoveredOnPosition;
            Validity<Vec2<int>> clickedPieceInitalPosition;
            Rectangle clickedPieceRectangle; // for drawing the piece held over the board
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
    void __drawBoard(bool flipped = false);
    void __flipBoard();

    bool __squareIsHoveredOn(Vec2<int> Position);
    bool __pieceIsClicked(Vec2<int> Position);

    void __handleMousePosition(int x, int y);
    void __handleMouseDownEvent(int x, int y);
    void __handleMouseUpEvent(int x, int y);

    void __clickOnPiece(int x, int y);
    void __releasePiece(int x, int y);
    void __updateClickedPieceRectanglePosition(int x, int y);
};
