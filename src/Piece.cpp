#include "Piece.h"

Piece::Piece()
    : __type(PieceEnum::None),
      __color(PieceColor::None)
{
}

Piece::Piece(PieceEnum Type)
: __type(Type)
{
    // TODO: make this in a better more generic way
    if (static_cast<int>(Type) >= 0 && static_cast<int>(Type) <= 5)
    {
        __color = PieceColor::Black;
    } else if (static_cast<int>(Type) >= 6 && static_cast<int>(Type) <= 11)
    {
        __color = PieceColor::White;
    }
    else {
        __color = PieceColor::None;
    }
}

Piece::Piece(char PieceType)
{
}
