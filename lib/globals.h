#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>

#include "RGBA_Color.h"
#include "Vec2.h"

// in case needed
#define NULL_STATEMENT ((void *)0) // statement that does nothing // the same as nullptr in C++

enum GameStatus : bool
{
    QuitGame,
    KeepRunning
};

namespace globals
{
    namespace piece
    {
        const std::string spritePath = "img/piece/ChessPieceSpriteSheetVer2.bmp";

        const std::array<std::string, 12> chessPieces = {
            "♔", // White King
            "♕", // White Queen
            "♖", // White Rook
            "♗", // White Bishop
            "♘", // White Knight
            "♙", // White Pawn
            "♚", // Black King
            "♛", // Black Queen
            "♜", // Black Rook
            "♝", // Black Bishop
            "♞", // Black Knight
            "♟"  // Black Pawn
        };
    };

    namespace board
    {
        const int ROWS = 8;
        const int COLS = ROWS;
        const int SQUARE_AMOUNT = ROWS * COLS;

        const std::string FEN_default = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        const std::string EMPTY_SQUARE_ATTACKED_PATH = "img/SolidCircleAttacked.bmp";
        const std::string OCCUPIED_SQUARE_ATTACKED_PATH = "img/HollowedCircleAttacked.bmp";

        const std::array<Vec2<int>, 4> DIAGONAL_DIRECTIONS = {
            Vec2<int>(1, 1),
            Vec2<int>(1, -1),
            Vec2<int>(-1, -1),
            Vec2<int>(-1, 1)

        };

        const std::array<Vec2<int>, 4> STRAIGHT_DIRECTIONS = {
            Vec2<int>(0, 1),
            Vec2<int>(-1, 0),
            Vec2<int>(0, -1),
            Vec2<int>(1, 0)

        };

        const std::array<Vec2<int>, 8> ALL_DIRECTIONS = {
            Vec2<int>(0, 1),
            Vec2<int>(0, -1),
            Vec2<int>(1, 0),
            Vec2<int>(-1, 0),
            Vec2<int>(1, 1),
            Vec2<int>(1, -1),
            Vec2<int>(-1, 1),
            Vec2<int>(-1, -1)

        };

        const std::array<Vec2<int>, 8> KNIGHT_DIRECTIONS = {
            Vec2<int>(+2, +1),
            Vec2<int>(+2, -1),
            Vec2<int>(-2, +1),
            Vec2<int>(-2, -1),
            Vec2<int>(+1, +2),
            Vec2<int>(+1, -2),
            Vec2<int>(-1, +2),
            Vec2<int>(-1, -2)

        };
    };

    namespace font
    {
        const int DEFAULT_FONT_SIZE = 12;
        
        namespace path {
            const std::string ROBOTO = "font/Roboto/";
        };

        namespace name {
            const std::string ROBOTO = "Roboto";

        };

        namespace type {
            const std::string REGULAR = "-Regular.ttf";
            const std::string BOLD = "-Bold.ttf";
        };
    };
    
    namespace window
    {
        const int WIDTH = 641;
        const int HEIGHT = WIDTH;

        const std::string NAME = "Chess";
    };

    namespace color
    {
        const RGBA_Color black = RGBA_Color(0, 0, 0);
        const RGBA_Color white = RGBA_Color(255, 255, 255);

        const RGBA_Color lightWhite = RGBA_Color(230, 234, 199);
        const RGBA_Color lightWhiteClicked = RGBA_Color(245, 245, 152);
        const RGBA_Color darkGreen = RGBA_Color(109, 141, 79);
        const RGBA_Color darkGreenClicked = RGBA_Color(191, 203, 95);

        const RGBA_Color darkTealGreen = RGBA_Color(0.2f, 0.3f, 0.3f);
    };
};
