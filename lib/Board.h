#pragma once

#include <string>
#include <vector>
#include "globals.h"
#include "Piece.h"

class Board
{
public:
    Board(const std::string &FEN = globals::board::FEN_default);
    ~Board();

    int getRows() const;
    int getCols() const;

    bool inBoard(const Vec2<int> position) const;
    bool squareOccupied(const Vec2<int> position) const;
    const Piece operator[](const Vec2<int> position) const;
    const Piece &at(const Vec2<int> position) const;

    template <typename FunctionObj>
    void iterateRowCol(FunctionObj Function) const;
    template <typename FunctionObj>
    void iterateColRow(FunctionObj Function) const;

private:
    struct FEN_data
    {
        Piece EmptyPiece;
        Piece *PiecePlacement[globals::board::ROWS][globals::board::COLS];
        PieceColor ActiveColor;
        std::string Castling;
        //? This is a square over which a pawn has just passed while moving two squares; it is given in algebraic notation. for example if e2 white pawn moved two squares to e4 then this variable will hold e3.
        std::string EnPassantTarget;
        //? The number of half-moves since the last capture or pawn advance, used for the fifty-move rule.
        int HalfMoveClock;
        //? It starts at 1 and is incremented after Black's move.
        int FullMoveNumber;
    };
    FEN_data __data;
    const int __rows, __cols;

    //? Forsyth-Edwards Notation Encoder
    FEN_data FEN_decoder(const std::string &FEN);
};

// templated functions' implementations here

template <typename FunctionObj>
void Board::iterateRowCol(FunctionObj Function) const
{
    int Rows = __rows, Cols = __cols;
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            Function(row, col);
        }
    }
}

template <typename FunctionObj>
void Board::iterateColRow(FunctionObj Function) const
{
    int Rows = __rows, Cols = __cols;
    for (int row = 0; row < Cols; row++)
    {
        for (int col = 0; col < Rows; col++)
        {
            Function(row, col);
        }
    }
}
