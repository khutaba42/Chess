#pragma once

#include "globals.h"
#include "Vec2.h"

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
    Piece(PieceEnum Type = PieceEnum::None, PieceColor Color = PieceColor::None);
    virtual ~Piece();

    PieceEnum type() const;
    PieceColor color() const;

private:
    PieceEnum __type;
    PieceColor __color;
};

//-----------------//-----------------//-----------------//-----------------//

class Pawn : public Piece 
{
public:
    Pawn(PieceColor Color);
    ~Pawn();
private:
    /* data */
};

//-----------------//-----------------//-----------------//-----------------//

class King : public Piece 
{
public:
    King(PieceColor Color);
    ~King();
private:
    /* data */
};

//-----------------//-----------------//-----------------//-----------------//


class Queen : public Piece 
{
public:
    Queen(PieceColor Color);
    ~Queen();
private:
    /* data */
};

//-----------------//-----------------//-----------------//-----------------//

class Bishop : public Piece 
{
public:
    Bishop(PieceColor Color);
    ~Bishop();
private:
    /* data */
};

//-----------------//-----------------//-----------------//-----------------//

class Knight : public Piece 
{
public:
    Knight(PieceColor Color);
    ~Knight();
private:
    /* data */
};

//-----------------//-----------------//-----------------//-----------------//

class Rook : public Piece 
{
public:
    virtual ~Rook();
    Rook(PieceColor Color);
private:
    /* data */
};

//-----------------//-----------------//-----------------//-----------------//

class KRook : public Rook 
{
public:
    KRook(PieceColor Color);
    ~KRook();

    bool hasMoved() const;
private:
    bool __hasMoved;
};

//-----------------//-----------------//-----------------//-----------------//

class QRook : public Rook 
{
public:
    QRook(PieceColor Color);
    ~QRook();

    bool hasMoved() const;
private:
    bool __hasMoved;
};

//-----------------//-----------------//-----------------//-----------------//

