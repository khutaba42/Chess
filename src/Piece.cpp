#include "Piece.h"

Piece::Piece(PieceEnum Type, PieceColor Color)
    : __type(Type),
      __color(Color)
{
}

Piece::~Piece()
{
}

PieceEnum Piece::type() const { return __type; }

PieceColor Piece::color() const { return __color; }

bool operator==(const Piece &a, const Piece &b) { return (a.__type == b.__type) && (a.__color == b.__color); }

bool operator!=(const Piece &a, const Piece &b) { return !(a == b); }
