#pragma once

#include "includes.h"
#include "Color.h"

enum class PieceEnum
{
    BlackPawn,
    BlackRook,
    BlackBishop,
    BlackKnight,
    BlackQueen,
    BlackKing,
    WhitePawn,
    WhiteRook,
    WhiteBishop,
    WhiteKnight,
    WhiteQueen,
    WhiteKing,
    None
};

enum class PieceColor
{
    White,
    Black,
    None
};

namespace globals
{
    namespace piece
    {
        const std::string spritePath = "img/piece/800px-Chess_Pieces_Sprite.svg.bmp";
        namespace ext
        {
            const std::string bmp = ".bmp";
            const std::string png = ".png";
        };
        const std::string Path = "img/piece/bmp/";

        const std::map<PieceEnum, std::string> base = {

            {PieceEnum::BlackPawn, "black_pawn"},
            {PieceEnum::BlackRook, "black_rook"},
            {PieceEnum::BlackBishop, "black_bishop"},
            {PieceEnum::BlackKnight, "black_knight"},
            {PieceEnum::BlackQueen, "black_queen"},
            {PieceEnum::BlackKing, "black_king"},
            {PieceEnum::WhitePawn, "white_pawn"},
            {PieceEnum::WhiteRook, "white_rook"},
            {PieceEnum::WhiteBishop, "white_bishop"},
            {PieceEnum::WhiteKnight, "white_knight"},
            {PieceEnum::WhiteQueen, "white_queen"},
            {PieceEnum::WhiteKing, "white_king"}

        };
    };

    namespace Board
    {
        const int ROWS = 8;
        const int COLS = ROWS;

        const std::string FEN_default = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    };

    namespace window
    {
        const int WIDTH = 640;
        const int HEIGHT = WIDTH;
    };

    namespace COLOR
    {
        const Color black = Color(0, 0, 0);
        const Color white = Color(255, 255, 255);

        const Color lightWhite = Color(230, 234, 199);
        const Color lightWhiteClicked = Color(245, 245, 152);
        const Color darkGreen = Color(109, 141, 79);
        const Color darkGreenClicked = Color(191, 203, 95);

        const Color darkTealGreen = Color(0.2f, 0.3f, 0.3f);
    };
};
