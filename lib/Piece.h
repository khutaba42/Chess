#pragma once

enum class PieceEnum
{
    Pawn,
    Rook,
    Bishop,
    Knight,
    Queen,
    King,
    None
};

enum class PieceColor
{
    White,
    Black,
    None
};

class Piece
{
public:
    Piece(PieceEnum Type = PieceEnum::None, PieceColor Color = PieceColor::None, bool hasMoved = false);
    ~Piece();

    PieceEnum type() const;
    PieceColor color() const;
    bool hasMoved() const;

    void move();

    friend bool operator==(const Piece &a, const Piece &b);
    friend bool operator!=(const Piece &a, const Piece &b);

private:
    PieceEnum __type;
    PieceColor __color;

    bool __hasMoved;
};
