#pragma once

class CastlingData
{
public:
    CastlingData();
    CastlingData(bool KingSideCastlingAvailable, bool QueenSideCastlingAvailable);
    ~CastlingData();

    bool canKingCastleAnySide() const;
    bool canKingCastleKingSide() const;
    bool canKingCastleQueenSide() const;

    void kingSideCastlingAvailable();
    void queenSideCastlingAvailable();
    void kingMoved();
    void KingSideRookMoved();
    void QueenSideRookMoved();

private:
    bool __kingSideCastlingAvailable;
    bool __queenSideCastlingAvailable;
};
