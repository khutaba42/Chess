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

bool Piece::is(const Piece &other) const { return this == &other; }
bool operator==(const Piece &a, const Piece &b) { return (a.__type == b.__type) && (a.__color == b.__color); }
bool operator!=(const Piece &a, const Piece &b) { return !(a == b); }

//-----------------//-----------------//-----------------//-----------------//

Pawn::Pawn(PieceColor Color) : Piece(PieceEnum::Pawn, Color), __hasMoved(false)
{
}

Pawn::~Pawn()
{
}

//-----------------//-----------------//-----------------//-----------------//

King::King(PieceColor Color) : Piece(PieceEnum::King, Color)
{
}

King::~King()
{
}

//-----------------//-----------------//-----------------//-----------------//

Queen::Queen(PieceColor Color) : Piece(PieceEnum::Queen, Color)
{
}

Queen::~Queen()
{
}

//-----------------//-----------------//-----------------//-----------------//

Bishop::Bishop(PieceColor Color) : Piece(PieceEnum::Bishop, Color)
{
}

Bishop::~Bishop()
{
}

//-----------------//-----------------//-----------------//-----------------//

Knight::Knight(PieceColor Color) : Piece(PieceEnum::Knight, Color)
{
}

Knight::~Knight()
{
}

//-----------------//-----------------//-----------------//-----------------//

Rook::Rook(PieceColor Color) : Piece(PieceEnum::Rook, Color)
{
}

Rook::~Rook()
{
}

//-----------------//-----------------//-----------------//-----------------//

KRook::KRook(PieceColor Color) : Rook(Color), __hasMoved(false)
{
}

KRook::~KRook()
{
}

bool KRook::hasMoved() const { return __hasMoved; }

//-----------------//-----------------//-----------------//-----------------//

QRook::QRook(PieceColor Color) : Rook(Color), __hasMoved(false)
{
}

QRook::~QRook()
{
}

bool QRook::hasMoved() const { return __hasMoved; }

//-----------------//-----------------//-----------------//-----------------//
