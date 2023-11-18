#pragma once

#include "includes.h"
#include "globals.h"
#include "Piece.h"

class Board
{
public:
    Board(const std::string &FEN = globals::Board::FEN_default);
    ~Board();

    bool isEmpty(const Vec2<int> position) const;
    const Piece operator[](const Vec2<int> position) const;

private:
    Piece* __piecePlacement[globals::Board::ROWS][globals::Board::COLS];
    //std::vector<std::vector<std::pair<Piece&, Vec2<int> > > > __piecePlacement;
    PieceColor __activeColor;
    std::string __castling;
    //? This is a square over which a pawn has just passed while moving two squares; it is given in algebraic notation. for example if e2 white pawn moved two squares to e4 then this variable will hold e3.
    std::string __EnPassantTarget;
    //? The number of halfmoves since the last capture or pawn advance, used for the fifty-move rule.
    int __HalfmoveClock;
    //? It starts at 1 and is incremented after Black's move.
    int __FullmoveNumber;

    //? Forsyth-Edwards Notation Encoder
    void FEN_decoder(const std::string &FEN);
};
