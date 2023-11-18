#pragma once
#include "includes.h"
#include "globals.h"
#include "Vec2.h"

class Piece
{
public:
    Piece();
    Piece(PieceEnum Type);
    Piece(char PieceType);

    PieceEnum type() const { return __type; }

private:
    PieceEnum __type;
    PieceColor __color;
};
