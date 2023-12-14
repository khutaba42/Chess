#pragma once

#include <string>
#include <vector>
#include "globals.h"
#include "Piece.h"
#include "Validity.h"

#include "CastlingData.h"

class Board
{
public:
    Board(const std::string &FEN = globals::board::FEN_default);
    ~Board();

    int getRows() const;
    int getCols() const;
    PieceColor getActiveColor() const;

    bool inBoard(const Vec2<int> position) const;
    bool squareOccupied(const Vec2<int> position) const;
    const Piece operator[](const Vec2<int> position) const;
    const Piece &at(const Vec2<int> position) const;

    void swapPieces(const Vec2<int> a, const Vec2<int> b);

    struct AttackedSquares
    {
        std::vector<Vec2<int>> empty;
        std::vector<Vec2<int>> occupied;
    };
    AttackedSquares getAllSquaresPieceCanGoTo(const Vec2<int> Square) const;

    /**
     * ! DO NOT mix up these enums ;
     *      the order has some significance in calculating the results to some functions.
     */
    enum class PieceMovementStatus
    {
        // ! these return a board Unchanged

        // return when the From position is out of the board bounds, board is unchanged.
        FromPositionNotInBoard,
        // return when the To position is out of the board bounds, board is unchanged.
        ToPositionNotInBoard,
        // return when the From AND To positions are out of the board bounds, board is unchanged.
        FromAndToPositionNotInBoard,
        // return when the From AND To positions are same position, board is unchanged.
        FromAndToSquareAreTheSameSquare,
        // return when the the From position is DOESN'T have a piece to move, board is unchanged.
        FromSquareNotOccupied,
        // return when the piece trying to move is NOT of the current active color, board is unchanged.
        FromHasNonActiveColorPiece,
        // return when the positions trying to move to IS of the current active color, board is unchanged.
        ToHasActiveColorPiece,
        // return when the From piece can't move to the To square because of the piece movement rules, board is unchanged.
        IllegalMove_MoveRevokesPieceMovementRules,
        // return when active color king is in check and moving this piece DOESN'T resolve that, board is unchanged.
        IllegalMove_ActiveColorKingIsInCheck,
        IllegalMove_ActiveColorKingGetsChecked,
        // return when moving this piece results in a check for the active color (the color of the current turn), board is unchanged.
IllegalMove_CastlingKingSideCantBeDone,
        // return when moving an active color king in castling through a checked square (returned for the checked squares in-between AND/OR the destination square), board is unchanged.
IllegalMove_CastlingQueenSideCantBeDone,
        // return when the active color king has moved and is trying to be castled, this has a higher priority than `IllegalMove_CastlingKingKingRookMoved` or `IllegalMove_CastlingKingQueenRookMoved`, board is unchanged.
IllegalMove_EnPassantCantBeDone,

        // ! these return a board changed

        // return when a pawn has been captured through en passant, this has a higher priority than `MovedToAnEmptySquare`, board is changed.
        PawnCapturedThroughEnPassant,
        // return when the (active color) king castle to the king-side successfully, board is changed.
        KingSideCastling,
        // return when the (active color) king castle to the queen-side successfully, board is changed.
        QueenSideCastling,
        // return when the moved piece is moved to an empty square (NOT returned when there is an EnPassant), board is changed.
        MovedToAnEmptySquare,
        // return when a piece gets captured successfully, board is changed.
        CapturedPiece,

        // ! an unexpected status

        // board is unchanged.
        UnknownStatus
    };
    PieceMovementStatus movePiece(const Vec2<int> From, const Vec2<int> To);
    bool boardHasChangedByStatus(const PieceMovementStatus status);
    bool isKingInCheck(PieceColor KingColor) const;

private:
    struct FEN_data
    {
        Piece EmptyPiece;

        /* *
         *         the {0, 0} position represents a8
         *     and the {8, 8} position represents h1
         */
        Piece PiecePlacement[globals::board::ROWS][globals::board::COLS];

        // static_cast<size_t>(PieceColor::None) gives the number of colors
        std::array<Validity<Vec2<int>>, static_cast<size_t>(PieceColor::None)> KingSquare;
        std::array<unsigned int, static_cast<size_t>(PieceColor::None)> PieceAmount;

        // -*- FEN data -*- //
        PieceColor ActiveColor;

        // ? Castling Availability
        std::array<CastlingData, static_cast<size_t>(PieceColor::None)> CastlingAvailability;

        //? This is a square over which a pawn has just passed while moving two squares; it is given in algebraic notation. for example if e2 white pawn moved two squares to e4 then this variable will hold e3.
        Validity<Vec2<int>> EnPassantTarget;

        //? The number of half-moves since the last capture or pawn advance, used for the fifty-move rule.
        int HalfMoveClock;

        //? It starts at 1 and is incremented after Black's move.
        int FullMoveNumber;
        // -*- FEN data -*- //
    };
    FEN_data __data;
    const int __rows, __cols;

    //? Forsyth-Edwards Notation Encoder
    FEN_data FEN_decoder(const std::string &FEN);

    /*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

    // ? king checks
    bool __isKingInCheck(PieceColor KingColor, bool CheckForKingChecks) const;

    // ? piece available moves
    AttackedSquares __getAllSquaresPieceCanGoTo(const Vec2<int> From) const;
    AttackedSquares __getAllSquaresKingCanGoTo(PieceColor Color) const;
    // returns the 2 diagonal squares a pawn can attack (beware to check for everything outside the function and the returns might not be inside the board bounds)
    std::pair<Vec2<int>, Vec2<int>> __NO_SAFETY__getPawnAttackedSquares(const Vec2<int> Pos) const;
    std::pair<Vec2<int>, Vec2<int>> __NO_SAFETY__getPawnAttackedSquares(const Vec2<int> Pos, const PieceColor Color) const;
    // returns the 2 ahead squares a pawn can attack (beware to check for everything outside the function and the returns might not be inside the board bounds)
    std::pair<Vec2<int>, Vec2<int>> __NO_SAFETY__getPawnMarchingSquares(const Vec2<int> Pos) const;

    // ? piece movement
    bool __NO_SAFETY__doesPieceMovementRulesAllowPieceToMove(const Vec2<int> From, const Vec2<int> To);
    // doesn't check if `KingColor` is not `None` and assumes board has a valid king square.
    friend bool __NO_SAFETY__isKingInCheck(const Board &board, PieceColor KingColor, bool CheckForKingChecks);

    // ? castling
    Vec2<int> __get_king_initial_square(PieceColor Color) const;

    Vec2<int> __get_kingSide_rook_square(PieceColor Color) const;
    Vec2<int> __get_queenSide_rook_square(PieceColor Color) const;

    Vec2<int> __get_kingSide_rook_castling_square(PieceColor Color) const;
    Vec2<int> __get_queenSide_rook_castling_square(PieceColor Color) const;

    Vec2<int> __get_kingSide_king_castling_square(PieceColor Color) const;
    Vec2<int> __get_queenSide_king_castling_square(PieceColor Color) const;

    bool __isKingSideCastlingSquare(Vec2<int> Square, PieceColor Color) const;
    bool __isQueenSideCastlingSquare(Vec2<int> Square, PieceColor Color) const;

    bool __NO_SAFETY__canKingCastleKingSide(PieceColor Color) const;
    bool __NO_SAFETY__canKingCastleQueenSide(PieceColor Color) const;

    // ? en passant
    Validity<Vec2<int>> __getEnPassantTarget() const;

    enum class MoveType
    {
        CastlingKingSide,
        CastlingQueenSide,
        EnPassant,
        Capture,
        None // returned when none of the above gets satisfied
    };
    MoveType __move_type(const Vec2<int> From, const Vec2<int> To) const; // * no need to return a color because the move is for the active color

    // ? moving pieces
    void __NO_SAFETY__movePiece(const Vec2<int> From, const Vec2<int> To, bool ChangeActiveColor = true);
    void __NO_SAFETY__castleKingSide(PieceColor Color);
    void __NO_SAFETY__castleQueenSide(PieceColor Color);
    void __NO_SAFETY__enPassant(const Vec2<int> From);

    //
    PieceColor __previousColorToPlay() const;
    PieceColor __nextColorToPlay() const;

    /*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/
};
