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
    PieceColor getActiveColor() const;

    bool inBoard(const Vec2<int> position) const;
    bool squareOccupied(const Vec2<int> position) const;
    const Piece operator[](const Vec2<int> position) const;
    const Piece &at(const Vec2<int> position) const;

    void swapPieces(const Vec2<int> a, const Vec2<int> b);
    std::vector<Vec2<int>> getPieceAttackingSquares(const Vec2<int> Square) const;

    enum PieceMovementStatus
    {
        FromPositionNotInBoard,
        ToPositionNotInBoard,
        FromAndToPositionNotInBoard,

        FromAndToSquareAreTheSameSquare,

        FromSquareNotOccupied,

        FromHasNonActiveColorPiece,

        ToHasActiveColorPiece,

        MovedToAnEmptySquare,
        CapturedPiece,
        IllegalMoveKingIsInCheck,
        IllegalMoveCantMoveThere,

        UnknownStatus
    };
    PieceMovementStatus movePiece(const Vec2<int> From, const Vec2<int> To);
    bool isKingInCheck(const Board &board, PieceColor KingColor);

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
