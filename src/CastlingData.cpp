#include "CastlingData.h"

CastlingData::CastlingData()
    : __kingSideCastlingAvailable(false),
      __queenSideCastlingAvailable(false)
{
}

CastlingData::CastlingData(bool KingSideCastlingAvailable, bool QueenSideCastlingAvailable)
    : __kingSideCastlingAvailable(KingSideCastlingAvailable),
      __queenSideCastlingAvailable(QueenSideCastlingAvailable)
{
}

CastlingData::~CastlingData() {}

bool CastlingData::canKingCastleAnySide() const
{
    return canKingCastleKingSide() || canKingCastleQueenSide();
}

bool CastlingData::canKingCastleKingSide() const
{
    return __kingSideCastlingAvailable;
}

bool CastlingData::canKingCastleQueenSide() const
{
    return __queenSideCastlingAvailable;
}

void CastlingData::kingSideCastlingAvailable()
{
    __kingSideCastlingAvailable = true;
}

void CastlingData::queenSideCastlingAvailable()
{
    __queenSideCastlingAvailable = true;
}

void CastlingData::kingMoved()
{
    __kingSideCastlingAvailable = false;
    __queenSideCastlingAvailable = false;
}

void CastlingData::KingSideRookMoved()
{
    __kingSideCastlingAvailable = false;
}

void CastlingData::QueenSideRookMoved()
{
    __queenSideCastlingAvailable = false;
}
