#include "Piece.h"

Piece::Piece(PieceEnum Type, PieceColor Color, bool hasMoved)
    : __type(Type),
      __color(Color),
      __hasMoved(hasMoved)
{
}

Piece::~Piece()
{
}

PieceEnum Piece::type() const { return __type; }

PieceColor Piece::color() const { return __color; }

bool Piece::hasMoved() const { return __hasMoved; }

void Piece::move() { __hasMoved = true; }

bool operator==(const Piece &a, const Piece &b) { return (a.__type == b.__type) && (a.__color == b.__color); }

bool operator!=(const Piece &a, const Piece &b) { return !(a == b); }
