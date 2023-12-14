#include "Board.h"

Board::Board(const std::string &FEN)
    : __data(FEN_decoder(FEN)),
      __rows(globals::board::ROWS),
      __cols(globals::board::COLS)
{
}

Board::~Board()
{
}

int Board::getRows() const { return __rows; }
int Board::getCols() const { return __cols; }
PieceColor Board::getActiveColor() const { return __data.ActiveColor; }

bool Board::inBoard(const Vec2<int> position) const
{
    return !(position.row < 0 || position.row >= __rows || position.col < 0 || position.col >= __cols);
}

bool Board::squareOccupied(const Vec2<int> position) const
{
    return inBoard(position) && (__data.PiecePlacement[position.row][position.col] != this->__data.EmptyPiece);
}

const Piece Board::operator[](const Vec2<int> position) const
{
    return __data.PiecePlacement[position.row][position.col];
}

const Piece &Board::at(const Vec2<int> position) const
{
    return __data.PiecePlacement[position.row][position.col];
}

void Board::swapPieces(const Vec2<int> a, const Vec2<int> b)
{
    if (inBoard(a) && inBoard(b))
    {
        std::swap(__data.PiecePlacement[a.row][a.col], __data.PiecePlacement[b.row][b.col]);
    }
    return;
}

Board::AttackedSquares Board::getAllSquaresPieceCanGoTo(const Vec2<int> Square) const
{
    return __getAllSquaresPieceCanGoTo(Square);
}

// Board::PieceMovementStatus Board::movePiece(const Vec2<int> From, const Vec2<int> To)
// {
//     //? Variables for the Do Nothing statements
//     bool FromInBoard = this->inBoard(From);
//     bool ToInBoard = this->inBoard(To);

//     bool FromAndToSquareAreTheSame = From == To;

//     bool FromSquareOccupied = this->squareOccupied(From);
//     //? Variables for the Mixed statements

//     //? Variables for the Do Something statements

//     /*---------------------------------------------------------------*/

//     /**
//      *    * Do Nothing statements,
//      *  returns the reason the board remains unchanged
//      */
//     // check if they are in-board positions
//     if (!FromInBoard || !ToInBoard)
//     {
//         if (FromInBoard && !ToInBoard)
//         {
//             return ToPositionNotInBoard;
//         }
//         else if (!FromInBoard && ToInBoard)
//         {
//             return FromPositionNotInBoard;
//         }
//         else // if(!FromInBoard && !ToInBoard)
//         {
//             return FromAndToPositionNotInBoard;
//         }
//     }
//     // check if From has a piece in it (to move)
//     else if (!FromSquareOccupied)
//     {
//         return FromSquareNotOccupied;
//     }
//     // check if the piece we are moving (from) is of the active color
//     else if (this->at(From).color() != __data.ActiveColor)
//     {
//         return FromHasNonActiveColorPiece;
//     }
//     // check if From and To are the same square
//     else if (!FromAndToSquareAreTheSame)
//     {
//         return FromAndToSquareAreTheSameSquare;
//     }
//     // check if the move in En passant
//     // else if ()
//     // {
//     //     return ;
//     // }
//     /**
//      *    * Mixed statements,
//      *  returns the reason the board changed after the change has been done
//      *  or
//      *  the reason the board remained unchanged.
//      */

//     // check if the piece we are moving to is NOT of the active color
//     else if (this->at(To).color() == __data.ActiveColor)
//     {
//         return ToHasActiveColorPiece;
//     }
//     /**
//      *    * Do Something statements,
//      *  returns the reason the board changed after the change has been done.
//      */
//     return UnknownStatus;
// }

Board::PieceMovementStatus Board::movePiece(const Vec2<int> From, const Vec2<int> To)
{
    Board &board = *this;

    bool FromInBoard = this->inBoard(From);
    bool ToInBoard = this->inBoard(To);

    /*---------------------------------------------------------------*/

    // check if `From` and\or `To` are in-board positions
    if (!FromInBoard && !ToInBoard)
    {
        return PieceMovementStatus::FromAndToPositionNotInBoard;
    }
    else if (!FromInBoard && ToInBoard)
    {
        return PieceMovementStatus::FromPositionNotInBoard;
    }
    else if (FromInBoard && !ToInBoard)
    {
        return PieceMovementStatus::ToPositionNotInBoard;
    }
    // check if From and To are the same square
    // TODO: for chess960 king can castle yet stay on the same square, implement that.
    else if (From == To)
    {
        return PieceMovementStatus::FromAndToSquareAreTheSameSquare;
    }
    // check if `From` has a piece in it (to move)
    else if (!board.squareOccupied(From))
    {
        return PieceMovementStatus::FromSquareNotOccupied;
    }
    // check if the piece we are moving (from) is of the active color
    else if (board.at(From).color() != __data.ActiveColor)
    {
        return PieceMovementStatus::FromHasNonActiveColorPiece;
    }
    // check if the piece we are moving to is NOT of the active color
    else if (board.at(To).color() == __data.ActiveColor)
    {
        return PieceMovementStatus::ToHasActiveColorPiece;
    }
    else if (!__NO_SAFETY__doesPieceMovementRulesAllowPieceToMove(From, To))
    {
        return PieceMovementStatus::IllegalMove_MoveRevokesPieceMovementRules;
    }
    else
    {
        /**
         * * inside this else block all the following assumptions hold:
         *  1. `From` AND `To` positions are inside the board bounds.
         *  2. `From` position in not the same position as `To`.
         *  3. `From` has a piece of the active color to move to `To`.
         *  4. `To` has NO piece of the active color (`To` is empty or has another color) therefore the From piece can move there
         *  5. piece follow the piece moving rules when moving to `To`.
         *
         * * After this point all moves should be allowed unless:
         *  1. Active Color King is in check and if the piece moved it wouldn't resolve the check.
         *  2. Moving this active color piece would cause a check for the active color king.
         *  3. En passant failure.
         *  4. Castling failure.
         */

        // check what type of move this is, castling, en passant or "normal" move
        MoveType move_type = board.__move_type(From, To);
        // create a test board
        Board PotentialBoard = *this;
        const PieceColor current_active_color = PotentialBoard.getActiveColor();

        // if king is currently under check, he must move or be protected by a piece
        if (board.__isKingInCheck(board.getActiveColor(), false)) /* false here because we assume the board accepts a move (game NOT over) */
        {
            if (move_type == MoveType::CastlingKingSide)
            {
                PotentialBoard.__NO_SAFETY__castleKingSide(current_active_color);
            }
            else if (move_type == MoveType::CastlingQueenSide)
            {
                PotentialBoard.__NO_SAFETY__castleQueenSide(current_active_color);
            }
            else if (move_type == MoveType::EnPassant)
            {
                PotentialBoard.__NO_SAFETY__enPassant(From);
            }
            else
            {
                PotentialBoard.__NO_SAFETY__movePiece(From, To);
            }

            if (PotentialBoard.__isKingInCheck(current_active_color, true))
            {
                // if king is still in check after the piece moved then the move doesn't resolve the check
                if (move_type == MoveType::CastlingKingSide)
                {
                    return PieceMovementStatus::IllegalMove_CastlingKingSideCantBeDone;
                }
                else if (move_type == MoveType::CastlingQueenSide)
                {
                    return PieceMovementStatus::IllegalMove_CastlingQueenSideCantBeDone;
                }
                else if (move_type == MoveType::EnPassant)
                {
                    return PieceMovementStatus::IllegalMove_EnPassantCantBeDone;
                }
                else if (move_type == MoveType::Capture)
                {
                    return PieceMovementStatus::IllegalMove_ActiveColorKingIsInCheck;
                }
                else
                {
                    return PieceMovementStatus::IllegalMove_ActiveColorKingIsInCheck;
                }
            }
            else // the move resolved the check
            {
                if (move_type == MoveType::CastlingKingSide)
                {
                    board.__NO_SAFETY__castleKingSide(board.getActiveColor());
                    return PieceMovementStatus::KingSideCastling;
                }
                else if (move_type == MoveType::CastlingQueenSide)
                {
                    board.__NO_SAFETY__castleQueenSide(board.getActiveColor());
                    return PieceMovementStatus::QueenSideCastling;
                }
                else if (move_type == MoveType::EnPassant)
                {
                    board.__NO_SAFETY__enPassant(From);
                    return PieceMovementStatus::PawnCapturedThroughEnPassant;
                }
                else if (move_type == MoveType::Capture)
                {
                    board.__NO_SAFETY__movePiece(From, To);
                    return PieceMovementStatus::CapturedPiece;
                }
                else
                {
                    board.__NO_SAFETY__movePiece(From, To);
                    return PieceMovementStatus::MovedToAnEmptySquare;
                }
            }
        }
        // king is NOT checked
        else
        {
            if (move_type == MoveType::CastlingKingSide)
            {
                PotentialBoard.__NO_SAFETY__castleKingSide(current_active_color);
            }
            else if (move_type == MoveType::CastlingQueenSide)
            {
                PotentialBoard.__NO_SAFETY__castleQueenSide(current_active_color);
            }
            else if (move_type == MoveType::EnPassant)
            {
                PotentialBoard.__NO_SAFETY__enPassant(From);
            }
            else
            {
                PotentialBoard.__NO_SAFETY__movePiece(From, To);
            }

            if (PotentialBoard.__isKingInCheck(current_active_color, true))
            {
                // king gets checked
                if (move_type == MoveType::CastlingKingSide)
                {
                    return PieceMovementStatus::IllegalMove_CastlingKingSideCantBeDone;
                }
                else if (move_type == MoveType::CastlingQueenSide)
                {
                    return PieceMovementStatus::IllegalMove_CastlingQueenSideCantBeDone;
                }
                else if (move_type == MoveType::EnPassant)
                {
                    return PieceMovementStatus::IllegalMove_EnPassantCantBeDone;
                }
                else if (move_type == MoveType::Capture)
                {
                    return PieceMovementStatus::IllegalMove_ActiveColorKingGetsChecked;
                }
                else
                {
                    return PieceMovementStatus::IllegalMove_ActiveColorKingGetsChecked;
                }
            }
            else // there were no checks
            {
                if (move_type == MoveType::CastlingKingSide)
                {
                    board.__NO_SAFETY__castleKingSide(board.getActiveColor());
                    return PieceMovementStatus::KingSideCastling;
                }
                else if (move_type == MoveType::CastlingQueenSide)
                {
                    board.__NO_SAFETY__castleQueenSide(board.getActiveColor());
                    return PieceMovementStatus::QueenSideCastling;
                }
                else if (move_type == MoveType::EnPassant)
                {
                    board.__NO_SAFETY__enPassant(From);
                    return PieceMovementStatus::PawnCapturedThroughEnPassant;
                }
                else if (move_type == MoveType::Capture)
                {
                    board.__NO_SAFETY__movePiece(From, To);
                    return PieceMovementStatus::CapturedPiece;
                }
                else
                {
                    board.__NO_SAFETY__movePiece(From, To);
                    return PieceMovementStatus::MovedToAnEmptySquare;
                }
            }
        }
    }
    return PieceMovementStatus::UnknownStatus;
}

bool Board::isKingInCheck(PieceColor KingColor) const
{
    return __isKingInCheck(KingColor, false);
}

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

enum CastlingLetters
{
    WhiteKingSideCastling = 'K',
    WhiteQueenSideCastling = 'Q',
    BlackKingSideCastling = 'k',
    BlackQueenSideCastling = 'q'
};

enum PieceLetters
{
    WhitePawn = 'P',
    WhiteRook = 'R',
    WhiteBishop = 'B',
    WhiteKnight = 'N',
    WhiteQueen = 'Q',
    WhiteKing = 'K',
    BlackPawn = 'p',
    BlackRook = 'r',
    BlackBishop = 'b',
    BlackKnight = 'n',
    BlackQueen = 'q',
    BlackKing = 'k'
};

Board::FEN_data Board::FEN_decoder(const std::string &FEN)
{
    /**
     * 0 - Piece placement
     * 1 - Active color
     * 2 - Castling availability
     * 3 - En passant target square
     * 4 - Half Move clock
     * 5 - Full Move number
     */
    Board::FEN_data data;

    std::string piece_positions_str;
    std::string active_color_str;
    std::string castling_str;
    std::string en_passant_str;
    std::string half_move_clock_str;
    std::string full_move_number_str;

    unsigned short currentStringPart = 0;
    for (const char letter : FEN)
    {
        if (letter == ' ') // space denotes a new field
        {
            currentStringPart++;
        }
        else
        {
            switch (currentStringPart)
            {
            case 0:
                piece_positions_str.push_back(letter);
                break;
            case 1:
                active_color_str.push_back(letter);
                break;
            case 2:
                castling_str.push_back(letter);
                break;
            case 3:
                en_passant_str.push_back(letter);
                break;
            case 4:
                half_move_clock_str.push_back(letter);
                break;
            case 5:
                full_move_number_str.push_back(letter);
                break;
            default:
                throw std::logic_error("too many spaces in FEN string.");
                break;
            }
        }
    }

    int Rows = globals::board::ROWS;
    int Cols = globals::board::COLS;

    Vec2<int> piece_placement = {0, 0};

    // * piece_positions_str translation
    for (const char letter : piece_positions_str)
    {
        if (letter >= '1' && letter < '8')
        {
            piece_placement.col += static_cast<int>(letter) - static_cast<int>('1') + 1;
            continue;
        }
        else if (letter == '8')
        {
            continue;
        }
        else if (letter == '/')
        {
            piece_placement.row++;
            piece_placement.col = 0;
            continue;
        }
        else
        {
            switch (letter)
            {
            case PieceLetters::WhitePawn:
                data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Pawn, PieceColor::White);
                break;
            case PieceLetters::WhiteKnight:
                data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Knight, PieceColor::White);
                break;
            case PieceLetters::WhiteBishop:
                data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Bishop, PieceColor::White);
                break;
            case PieceLetters::WhiteRook:
                data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Rook, PieceColor::White);
                break;
            case PieceLetters::WhiteQueen:
                data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Queen, PieceColor::White);
                break;
            case PieceLetters::WhiteKing:
                data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::King, PieceColor::White);
                data.KingSquare[static_cast<size_t>(PieceColor::White)] = piece_placement;
                break;
            case PieceLetters::BlackPawn:
                data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Pawn, PieceColor::Black);
                break;
            case PieceLetters::BlackKnight:
                data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Knight, PieceColor::Black);
                break;
            case PieceLetters::BlackBishop:
                data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Bishop, PieceColor::Black);
                break;
            case PieceLetters::BlackRook:
                data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Rook, PieceColor::Black);
                break;
            case PieceLetters::BlackQueen:
                data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::Queen, PieceColor::Black);
                break;
            case PieceLetters::BlackKing:
                data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                data.PiecePlacement[piece_placement.row][piece_placement.col] = Piece(PieceEnum::King, PieceColor::Black);
                data.KingSquare[static_cast<size_t>(PieceColor::Black)] = piece_placement;
                break;
            default:
                std::cout << "Unknown format of FEN in part 0 <" << letter << ">.\n";
                break;
            }
            piece_placement.col++;
        }
    }

    // * active_color_str translation
    for (const char letter : active_color_str)
    {
        if (letter == 'w')
        {
            data.ActiveColor = PieceColor::White;
        }
        else if (letter == 'b')
        {
            data.ActiveColor = PieceColor::Black;
        }
        else
        {
            std::cout << "Unknown format of FEN in part 1 <" << letter << ">.\n";
        }
    }

    // * castling_str translation
    for (const char letter : castling_str)
    {
        switch (letter)
        {
        case CastlingLetters::WhiteKingSideCastling:
            data.CastlingAvailability[static_cast<size_t>(PieceColor::White)].kingSideCastlingAvailable();
            break;
        case CastlingLetters::WhiteQueenSideCastling:
            data.CastlingAvailability[static_cast<size_t>(PieceColor::White)].queenSideCastlingAvailable();
            break;
        case CastlingLetters::BlackKingSideCastling:
            data.CastlingAvailability[static_cast<size_t>(PieceColor::Black)].kingSideCastlingAvailable();
            break;
        case CastlingLetters::BlackQueenSideCastling:
            data.CastlingAvailability[static_cast<size_t>(PieceColor::Black)].queenSideCastlingAvailable();
            break;
        case '-':
            // default constructor for `CastlingData`
            break;
        default:
            throw std::runtime_error("Unknown format of FEN in part 3 Castling\n");
            break;
        }
    }

    // * en_passant_str translation
    if (en_passant_str == "-")
    {
        __data.EnPassantTarget.valid = false;
        //__data.EnPassantTarge has garbage value;
    }
    else
    {
        Vec2<int> EnPassantSquare;
        EnPassantSquare.row = static_cast<int>(en_passant_str[0]) - static_cast<int>('a');
        EnPassantSquare.col = static_cast<int>(en_passant_str[1]) - static_cast<int>('1');
        __data.EnPassantTarget.valid = true;
        __data.EnPassantTarget.data = EnPassantSquare;
    }

    // * half_move_clock_str translation
    try
    {
        data.HalfMoveClock = std::stoi(half_move_clock_str);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Unknown format of FEN in part 4: Half move clock = " + half_move_clock_str + "\n"));
    }

    // * full_move_number_str translation
    try
    {
        data.FullMoveNumber = std::stoi(full_move_number_str);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Unknown format of FEN in part 5: Full move number = " + full_move_number_str + "\n"));
    }

    return data;
}

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

bool Board::__isKingInCheck(PieceColor KingColor, bool CheckForKingChecks) const
{
    const Board &board = *this;
    if (KingColor == PieceColor::None ||
        !board.__data.KingSquare[static_cast<size_t>(KingColor)].valid)
    {
        return false;
    }
    return __NO_SAFETY__isKingInCheck(*this, KingColor, CheckForKingChecks);
}

Board::AttackedSquares Board::__getAllSquaresPieceCanGoTo(const Vec2<int> From) const
{
    AttackedSquares Squares;

    if (this->inBoard(From))
    {
        const Piece piece = this->at(From);
        const PieceColor Color = piece.color();
        const PieceEnum Type = piece.type();
        const bool HasMoved = piece.hasMoved();

        switch (Type)
        {
        case PieceEnum::King:
            Squares = __getAllSquaresKingCanGoTo(Color);
            break;
        case PieceEnum::Pawn:
            do
            {
                auto pair = __NO_SAFETY__getPawnAttackedSquares(From);
                Vec2<int> first_diagonal = pair.first;
                Vec2<int> second_diagonal = pair.second;
                if (this->inBoard(first_diagonal) && this->at(first_diagonal).color() != Color)
                {
                    if (this->at(first_diagonal).color() != PieceColor::None)
                    {
                        Squares.occupied.push_back(first_diagonal);
                    }
                    else if (this->__getEnPassantTarget().valid && this->__getEnPassantTarget().data == first_diagonal) // && (this->at(first_diagonal).color() == PieceColor::None)
                    {
                        // there is en passant
                        Squares.empty.push_back(first_diagonal);
                    }
                }
                if (this->inBoard(second_diagonal) && this->at(second_diagonal).color() != Color)
                {
                    if (this->at(second_diagonal).color() != PieceColor::None)
                    {
                        Squares.occupied.push_back(second_diagonal);
                    }
                    else if (this->__getEnPassantTarget().valid && this->__getEnPassantTarget().data == second_diagonal) // && (this->at(second_diagonal).color() == PieceColor::None)
                    {
                        // there is en passant
                        Squares.empty.push_back(second_diagonal);
                    }
                }

                pair = __NO_SAFETY__getPawnMarchingSquares(From);
                Vec2<int> first_ahead = pair.first;
                Vec2<int> second_ahead = pair.second;
                if (this->inBoard(first_ahead) && this->at(first_ahead).color() == PieceColor::None)
                {
                    Squares.empty.push_back(first_ahead);
                    if (!HasMoved && this->inBoard(second_ahead) && this->at(second_ahead).color() == PieceColor::None)
                    {
                        Squares.empty.push_back(second_ahead);
                    }
                }

            } while (false);
            break;
        case PieceEnum::Bishop:
            for (Vec2<int> direction : globals::board::DIAGONAL_DIRECTIONS)
            {
                Vec2<int> Dest = From + direction;
                while (this->inBoard(Dest))
                {
                    PieceColor potentialSquareColor = this->at(Dest).color();
                    PieceEnum potentialSquareType = this->at(Dest).type();

                    if (potentialSquareColor == PieceColor::None) // no piece
                    {
                        Squares.empty.push_back(Dest);
                        Dest += direction;
                    }
                    else // Dest has a piece in it
                    {
                        if (potentialSquareColor != Color) // Dest has enemy color
                        {
                            Squares.occupied.push_back(Dest);
                        }
                        break; // from the while loop and continue to the next direction in the for loop to check
                    }
                }
            }
            break;
        case PieceEnum::Rook:
            for (Vec2<int> direction : globals::board::STRAIGHT_DIRECTIONS)
            {
                Vec2<int> Dest = From + direction;
                while (this->inBoard(Dest))
                {
                    PieceColor potentialSquareColor = this->at(Dest).color();
                    PieceEnum potentialSquareType = this->at(Dest).type();

                    if (potentialSquareColor == PieceColor::None) // no piece
                    {
                        Squares.empty.push_back(Dest);
                        Dest += direction;
                    }
                    else // Dest has a piece in it
                    {
                        if (potentialSquareColor != Color) // Dest has enemy color
                        {
                            Squares.occupied.push_back(Dest);
                        }
                        break; // from the while loop and continue to the next direction in the for loop to check
                    }
                }
            }
            break;
        case PieceEnum::Queen:
            for (Vec2<int> direction : globals::board::ALL_DIRECTIONS)
            {
                Vec2<int> Dest = From + direction;
                while (this->inBoard(Dest))
                {
                    PieceColor potentialSquareColor = this->at(Dest).color();
                    PieceEnum potentialSquareType = this->at(Dest).type();

                    if (potentialSquareColor == PieceColor::None) // no piece
                    {
                        Squares.empty.push_back(Dest);
                        Dest += direction;
                    }
                    else // Dest has a piece in it
                    {
                        if (potentialSquareColor != Color) // Dest has enemy color
                        {
                            Squares.occupied.push_back(Dest);
                        }
                        break; // from the while loop and continue to the next direction in the for loop to check
                    }
                }
            }
            break;
        case PieceEnum::Knight:
            for (Vec2<int> dir : globals::board::KNIGHT_DIRECTIONS)
            {
                Vec2<int> Dest = From + dir;
                if (this->inBoard(Dest) && this->at(Dest).color() != Color)
                {
                    if (this->squareOccupied(Dest))
                    {
                        Squares.occupied.push_back(Dest);
                    }
                    else
                    {
                        Squares.empty.push_back(Dest);
                    }
                }
            }
        default:
            break;
        }
    }
    return Squares;
}

// TODO: remove magic numbers
Vec2<int> Board::__get_king_initial_square(PieceColor Color) const
{
    switch (Color)
    {
    case PieceColor::White:
        return {7, 4};
        break;
    case PieceColor::Black:
        return {0, 4};
        break;
    default:
        throw std::logic_error("__get_king_initial_square cant have argument as PieceColor::None.");
        break;
    }
}

// TODO: remove magic numbers
Vec2<int> Board::__get_kingSide_rook_square(PieceColor Color) const
{
    switch (Color)
    {
    case PieceColor::White:
        return {7, 7};
        break;
    case PieceColor::Black:
        return {0, 7};
        break;
    default:
        throw std::logic_error("__get_kingSide_rook_square cant have argument as PieceColor::None.");
        break;
    }
}

// TODO: remove magic numbers
Vec2<int> Board::__get_queenSide_rook_square(PieceColor Color) const
{
    switch (Color)
    {
    case PieceColor::White:
        return {7, 0};
        break;
    case PieceColor::Black:
        return {0, 0};
        break;
    default:
        throw std::logic_error("__get_queenSide_rook_square cant have argument as PieceColor::None.");
        break;
    }
}

// TODO: remove magic numbers
Vec2<int> Board::__get_kingSide_rook_castling_square(PieceColor Color) const
{
    switch (Color)
    {
    case PieceColor::White:
        return Vec2<int>(7, 5);
        break;
    case PieceColor::Black:
        return Vec2<int>(0, 5);
        break;
    default:
        throw std::logic_error("__get_kingSide_king_castling_square cant have argument as PieceColor::None.");
        break;
    }
}

// TODO: remove magic numbers
Vec2<int> Board::__get_queenSide_rook_castling_square(PieceColor Color) const
{
    switch (Color)
    {
    case PieceColor::White:
        return Vec2<int>(7, 3);
        break;
    case PieceColor::Black:
        return Vec2<int>(0, 3);
        break;
    default:
        throw std::logic_error("__get_kingSide_king_castling_square cant have argument as PieceColor::None.");
        break;
    }
}

// TODO: remove magic numbers
Vec2<int> Board::__get_kingSide_king_castling_square(PieceColor Color) const
{
    switch (Color)
    {
    case PieceColor::White:
        return Vec2<int>(7, 6);
        break;
    case PieceColor::Black:
        return Vec2<int>(0, 6);
        break;
    default:
        throw std::logic_error("__get_kingSide_king_castling_square cant have argument as PieceColor::None.");
        break;
    }
}

// TODO: remove magic numbers
Vec2<int> Board::__get_queenSide_king_castling_square(PieceColor Color) const
{
    switch (Color)
    {
    case PieceColor::White:
        return Vec2<int>(7, 2);
        break;
    case PieceColor::Black:
        return Vec2<int>(0, 2);
        break;
    default:
        throw std::logic_error("__get_queenSide_king_castling_square cant have argument as PieceColor::None.");
        break;
    }
}

bool Board::__isKingSideCastlingSquare(Vec2<int> Square, PieceColor Color) const
{
    return __get_kingSide_king_castling_square(Color) == Square;
}

bool Board::__isQueenSideCastlingSquare(Vec2<int> Square, PieceColor Color) const
{
    return __get_queenSide_king_castling_square(Color) == Square;
}

bool Board::__NO_SAFETY__canKingCastleKingSide(PieceColor Color) const
{
    // if (Color == PieceColor::None)
    // {
    //     throw std::logic_error("__NO_SAFETY__canKingCastleKingSide cant have argument as PieceColor::None.");
    // }
    // if (!this->__data.KingSquare[static_cast<size_t>(Color)].valid)
    // {
    //     throw std::logic_error("__NO_SAFETY__canKingCastleKingSide there is no king of this color.");
    // }

    const Board &board = *this;

    const Vec2<int> king_square = this->__data.KingSquare[static_cast<size_t>(Color)].data;
    const Piece king_piece = board.at(king_square);

    const Vec2<int> king_side_rook_square = board.__get_kingSide_rook_square(Color);
    const Vec2<int> king_side_rook_dir = globals::board::KING_DIRECTIONS_KING_SIDE_CASTLING_DIRECTIONS[0];
    const Piece king_side_rook_piece = board.at(king_side_rook_square);

    if (king_piece.hasMoved())
    {
        return false; // king CAN'T castle
    }

    if (king_side_rook_piece.hasMoved())
    {
        return false; // king CAN'T castle king side
    }

    // check if all the squares between the king and the rook are empty
    // TODO: get rid of magic values
    Vec2<int> dest = king_square;
    for (size_t i = 0; i < 2; i++) // king side castling has 2 squares between the king and the rook
    {
        dest += king_side_rook_dir;
        if (board.squareOccupied(dest))
        {
            return false;
        }
    }

    // beyond this point we only need to check if the king doesn't walk through check when castling
    const Vec2<int> king_side_castling_square = board.__get_kingSide_king_castling_square(Color);
    Board ExpermentalBoard = board;
    ExpermentalBoard.__NO_SAFETY__movePiece(king_square, king_side_castling_square);
    if (__NO_SAFETY__isKingInCheck(ExpermentalBoard, board.getActiveColor(), true))
    {
        return false;
    }
    return true;
}

bool Board::__NO_SAFETY__canKingCastleQueenSide(PieceColor Color) const
{
    // if (Color == PieceColor::None)
    // {
    //     throw std::logic_error("__NO_SAFETY__canKingCastleQueenSide cant have argument as PieceColor::None.");
    // }
    // if (!this->__data.KingSquare[static_cast<size_t>(Color)].valid)
    // {
    //     throw std::logic_error("__NO_SAFETY__canKingCastleQueenSide there is no king of this color.");
    // }

    const Board &board = *this;

    const Vec2<int> king_square = this->__data.KingSquare[static_cast<size_t>(Color)].data;
    const Piece king_piece = board.at(king_square);

    const Vec2<int> queen_side_rook_square = board.__get_queenSide_rook_square(Color);
    const Vec2<int> queen_side_rook_dir = globals::board::KING_DIRECTIONS_QUEEN_SIDE_CASTLING_DIRECTIONS[0];
    const Piece queen_side_rook_piece = board.at(queen_side_rook_square);

    if (king_piece.hasMoved())
    {
        return false; // king CAN'T castle
    }

    if (queen_side_rook_piece.hasMoved())
    {
        return false; // king CAN'T castle queen side
    }

    // check if all the squares between the king and the rook are empty
    // TODO: get rid of magic values
    Vec2<int> dest = king_square;
    for (size_t i = 0; i < 3; i++) // king side castling has 3 squares between the king and the rook
    {
        dest += queen_side_rook_dir;
        if (board.squareOccupied(dest))
        {
            return false;
        }
    }

    // beyond this point we only need to check if the king doesn't walk through check when castling
    const Vec2<int> queen_side_castling_square = board.__get_queenSide_king_castling_square(Color);
    Board ExpermentalBoard = board;
    ExpermentalBoard.__NO_SAFETY__movePiece(king_square, queen_side_castling_square);
    if (__NO_SAFETY__isKingInCheck(ExpermentalBoard, board.getActiveColor(), true))
    {
        return false;
    }

    return true;
}

Validity<Vec2<int>> Board::__getEnPassantTarget() const
{
    return __data.EnPassantTarget;
}

// TODO: get rid of hard coded numbers
Board::AttackedSquares Board::__getAllSquaresKingCanGoTo(PieceColor Color) const
{
    if (Color == PieceColor::None)
    {
        throw std::logic_error("__canKingCastleKingSide cant have argument as PieceColor::None.");
    }

    bool king_square_valid = this->__data.KingSquare[static_cast<size_t>(Color)].valid;
    if (!king_square_valid)
    {
        throw std::logic_error("__canKingCastleKingSide there is no king of this color.");
    }

    // castling variables
    const Vec2<int> king_side_castling_dir = globals::board::KING_DIRECTIONS_KING_SIDE_CASTLING_DIRECTIONS[0];
    const Vec2<int> queen_side_castling_dir = globals::board::KING_DIRECTIONS_QUEEN_SIDE_CASTLING_DIRECTIONS[0];
    bool king_side_castling_adjacent_square_no_check = false;
    bool queen_side_castling_adjacent_square_no_check = false;

    AttackedSquares Squares;
    Vec2<int> king_square = this->__data.KingSquare[static_cast<size_t>(Color)].data;
    for (Vec2<int> dir : globals::board::ALL_DIRECTIONS)
    {
        Vec2<int> Dest = king_square + dir;

        if (this->inBoard(Dest) && this->at(Dest).color() != Color)
        {
            Board ExperimentBoard = *this;
            ExperimentBoard.__NO_SAFETY__movePiece(king_square, Dest);
            if (!__NO_SAFETY__isKingInCheck(ExperimentBoard, Color, true))
            {
                if (this->squareOccupied(Dest))
                {
                    Squares.occupied.push_back(Dest);
                }
                else
                {
                    Squares.empty.push_back(Dest);
                }

                if (dir == king_side_castling_dir)
                {
                    king_side_castling_adjacent_square_no_check = true;
                }

                if (dir == queen_side_castling_dir)
                {
                    queen_side_castling_adjacent_square_no_check = true;
                }
            }
        }
    }

    // TODO: Castling
    // ? King side castling
    if (king_side_castling_adjacent_square_no_check && __NO_SAFETY__canKingCastleKingSide(Color))
    {
        Vec2<int> Dest = king_square + globals::board::KING_DIRECTIONS_KING_SIDE_CASTLING_DIRECTIONS[1];
        Squares.empty.push_back(Dest);
    }

    // ? Queen side castling
    if (queen_side_castling_adjacent_square_no_check && __NO_SAFETY__canKingCastleQueenSide(Color))
    {

        Vec2<int> Dest = king_square + globals::board::KING_DIRECTIONS_QUEEN_SIDE_CASTLING_DIRECTIONS[1];
        Squares.empty.push_back(Dest);
    }

    return Squares;
}

bool Board::__NO_SAFETY__doesPieceMovementRulesAllowPieceToMove(const Vec2<int> From, const Vec2<int> To)
{
    AttackedSquares AllSquares = __getAllSquaresPieceCanGoTo(From);
    for (const Vec2<int> potentialSquare : AllSquares.empty)
    {
        if (potentialSquare == To)
        {
            return true;
        }
    }
    for (const Vec2<int> potentialSquare : AllSquares.occupied)
    {
        if (potentialSquare == To)
        {
            return true;
        }
    }
    // if the for loop is completed then To is one of these squares
    return false;
}

//  , ignore the `__NO_SAFETY_CHECK_` thing
bool __NO_SAFETY__isKingInCheck(const Board &board, PieceColor KingColor, bool CheckForKingChecks)
{

    Vec2<int> ActiveColorKingSquare = board.__data.KingSquare[static_cast<size_t>(KingColor)].data; // this holds the king squares info to get it in O(1)

    // knight -s
    for (Vec2<int> dir : globals::board::KNIGHT_DIRECTIONS) // squares a knight may from
    {
        Vec2<int> potentialSquare = ActiveColorKingSquare + dir; // get the places as squares
        if (board.inBoard(potentialSquare))                      // check if the square is inside the board bounds
        {
            PieceColor potentialSquareColor = board.at(potentialSquare).color();
            PieceEnum potentialSquareType = board.at(potentialSquare).type();

            if (potentialSquareColor != PieceColor::None && // square has a pice in it
                potentialSquareColor != KingColor &&        // square has enemy of king
                potentialSquareType == PieceEnum::Knight)   // square has enemy knight
            {
                return true; // king is in check by knight?
            }
        }
    }

    // Bishop or Queen checks
    for (Vec2<int> dir : globals::board::DIAGONAL_DIRECTIONS)
    {
        Vec2<int> potentialSquare = ActiveColorKingSquare + dir;
        while (board.inBoard(potentialSquare))
        {
            PieceColor potentialSquareColor = board.at(potentialSquare).color();
            PieceEnum potentialSquareType = board.at(potentialSquare).type();

            if (potentialSquareColor == KingColor)
            {
                break; // from the while loop and continue to the next direction in the for loop to check
            }
            else if (potentialSquareColor == PieceColor::None) // there is no piece
            {
                potentialSquare += dir;
            }
            else // potentialSquare has enemy color
            {
                if ((potentialSquareType == PieceEnum::Bishop) ||
                    (potentialSquareType == PieceEnum::Queen))
                {
                    return true; // king is in check
                }
                else
                {
                    break; // from the while loop and continue to the next direction in the for loop to check
                }
            }
        }
    }

    // Rook or Queen checks
    for (Vec2<int> dir : globals::board::STRAIGHT_DIRECTIONS)
    {
        Vec2<int> potentialSquare = ActiveColorKingSquare + dir;
        while (board.inBoard(potentialSquare))
        {
            PieceColor potentialSquareColor = board.at(potentialSquare).color();
            PieceEnum potentialSquareType = board.at(potentialSquare).type();

            if (potentialSquareColor == KingColor)
            {
                break; // from the while loop and continue to the next direction in the for loop to check
            }
            else if (potentialSquareColor == PieceColor::None) // there is no piece
            {
                potentialSquare += dir;
            }
            else // potentialSquare has enemy color
            {
                if ((potentialSquareType == PieceEnum::Rook) ||
                    (potentialSquareType == PieceEnum::Queen))
                {
                    return true; // king is in check
                }
                else
                {
                    break; // from the while loop and continue to the next direction in the for loop to check
                }
            }
        }
    }

    // pawn checks (for simplicity im gonna go through all directions)
    for (Vec2<int> dir : globals::board::DIAGONAL_DIRECTIONS)
    {
        Vec2<int> potentialSquare = ActiveColorKingSquare + dir;
        if (board.inBoard(potentialSquare))
        {
            PieceColor potentialSquareColor = board.at(potentialSquare).color();
            PieceEnum potentialSquareType = board.at(potentialSquare).type();

            if (potentialSquareColor != PieceColor::None &&
                potentialSquareColor != KingColor &&
                potentialSquareType == PieceEnum::Pawn)
            {
                auto two_squares = board.__NO_SAFETY__getPawnAttackedSquares(potentialSquare);
                if (two_squares.first == ActiveColorKingSquare ||
                    two_squares.second == ActiveColorKingSquare)
                {
                    return true; // king is in check
                }
            }
        }
    }

    // ! king checks
    if (CheckForKingChecks)
    {
        for (Vec2<int> dir : globals::board::ALL_DIRECTIONS)
        {
            Vec2<int> potentialSquare = ActiveColorKingSquare + dir;
            if (board.inBoard(potentialSquare))
            {
                PieceColor potentialSquareColor = board.at(potentialSquare).color();
                PieceEnum potentialSquareType = board.at(potentialSquare).type();

                if (potentialSquareColor != PieceColor::None &&
                    potentialSquareColor != KingColor &&
                    potentialSquareType == PieceEnum::King)
                {
                    return true; // king is in check
                }
            }
        }
    }

    return false; // king is NOT in check
}

std::pair<Vec2<int>, Vec2<int>> Board::__NO_SAFETY__getPawnAttackedSquares(const Vec2<int> Pos) const
{
    return __NO_SAFETY__getPawnAttackedSquares(Pos, this->at(Pos).color());
}

std::pair<Vec2<int>, Vec2<int>> Board::__NO_SAFETY__getPawnAttackedSquares(const Vec2<int> Pos, const PieceColor Color) const
{
    /**
     * {0, 0} position represents a8
     * {8, 8} position represents h1
     * black pawns go down (adding rows)
     * white pawns go up (subtracting rows)
     */
    switch (Color)
    {
    case PieceColor::Black:
        return {
            Pos + Vec2<int>(+1, +1),
            Pos + Vec2<int>(+1, -1)

        };
        break;
    case PieceColor::White:
        return {
            Pos + Vec2<int>(-1, +1),
            Pos + Vec2<int>(-1, -1)

        };
        break;
    default:
        throw std::logic_error("__NO_SAFETY__getPawnAttackedSquares got invalid argument");
        break;
    }
}

std::pair<Vec2<int>, Vec2<int>> Board::__NO_SAFETY__getPawnMarchingSquares(const Vec2<int> Pos) const
{
    /**
     * {0, 0} position represents a8
     * {8, 8} position represents h1
     * black pawns go down (adding rows)
     * white pawns go up (subtracting rows)
     */
    switch (this->at(Pos).color())
    {
    case PieceColor::Black:
        return {
            Pos + Vec2<int>(+1, 0),
            Pos + Vec2<int>(+2, 0)

        };
        break;
    case PieceColor::White:
        return {
            Pos + Vec2<int>(-1, 0),
            Pos + Vec2<int>(-2, 0)

        };
        break;
    default:
        throw std::logic_error("__NO_SAFETY__getPawnMarchingSquares got invalid argument.\n");
        break;
    }
}

PieceColor Board::__previousColorToPlay() const
{
    PieceColor next;
    switch (getActiveColor())
    {
    case PieceColor::Black:
        next = PieceColor::White;
        break;
    case PieceColor::White:
        next = PieceColor::Black;
        break;
    default:
        next = PieceColor::None;
        break;
    }
    return next;
}

PieceColor Board::__nextColorToPlay() const
{
    PieceColor next;
    switch (getActiveColor())
    {
    case PieceColor::Black:
        next = PieceColor::White;
        break;
    case PieceColor::White:
        next = PieceColor::Black;
        break;
    default:
        next = PieceColor::None;
        break;
    }
    return next;
}

Board::MoveType Board::__move_type(const Vec2<int> From, const Vec2<int> To) const
{
    const Board &board = *this;
    const PieceColor color_to_play = board.getActiveColor();

    // * castling
    bool from_is_the_kings_initial_square = (From == board.__get_king_initial_square(color_to_play));

    bool to_is_kingside_king_castling_square = (To == board.__get_kingSide_king_castling_square(color_to_play));
    bool to_is_queenside_king_castling_square = (To == board.__get_queenSide_king_castling_square(color_to_play));

    bool king_side_castling =
        from_is_the_kings_initial_square &&
        to_is_kingside_king_castling_square;

    bool queen_side_castling =
        from_is_the_kings_initial_square &&
        to_is_queenside_king_castling_square;

    if (king_side_castling)
    {
        return MoveType::CastlingKingSide;
    }

    if (queen_side_castling)
    {
        return MoveType::CastlingQueenSide;
    }

    // * en passant
    bool there_is_an_en_passant_target = __data.EnPassantTarget.valid;
    bool to_is_an_en_passant_target = (To == __data.EnPassantTarget.data);
    auto square_pair = __NO_SAFETY__getPawnAttackedSquares(To, __previousColorToPlay());
    bool from_has_a_pawn_attacking_the_en_passant_target = (From == square_pair.first) || (From == square_pair.second);
    bool from_has_a_pawn_of_the_color_to_play = (board.at(From).color() == color_to_play);

    bool en_passant_available =
        there_is_an_en_passant_target &&
        to_is_an_en_passant_target &&
        from_has_a_pawn_attacking_the_en_passant_target &&
        from_has_a_pawn_of_the_color_to_play;

    if (en_passant_available)
    {
        return MoveType::EnPassant;
    }

    // * other

    return MoveType::None;
}

void Board::__NO_SAFETY__movePiece(const Vec2<int> From, const Vec2<int> To, bool ChangeActiveColor)
{
    // update the necessary data if needed
    Board &board = *this;
    const Piece piece = board.at(From);
    const PieceEnum piece_type = piece.type();
    const PieceColor piece_color = piece.color();
    const bool piece_has_moved = piece.hasMoved();

    // first make the en passant target unavailable // TODO: explain why
    __data.EnPassantTarget.valid = false;
    // check for castling (and update board info about it) and update king info
    if (piece_type == PieceEnum::King)
    {
        // update the king square
        __data.KingSquare[static_cast<size_t>(piece_color)].data = To;
        // update the castling availability
        board.__data.CastlingAvailability[static_cast<size_t>(piece_color)].kingMoved();
    }
    // check for en passant and create en passant target , if the move is not a pawn moving 2 squares ahead then remove the en passant target
    else if (piece_type == PieceEnum::Pawn)
    {
        // if it is the pawn's first move
        if (!piece_has_moved)
        {
            // if the pawn moves 2 squares ahead then create en passant target
            auto marching_squares_pair = board.__NO_SAFETY__getPawnMarchingSquares(From);
            if (marching_squares_pair.second == To)
            {
                __data.EnPassantTarget.valid = true;
                __data.EnPassantTarget.data = marching_squares_pair.first;
            }
        }
    }

    // the piece that was in From is now in To.
    __data.PiecePlacement[To.row][To.col] = __data.PiecePlacement[From.row][From.col];
    // remove the piece that was in From
    __data.PiecePlacement[From.row][From.col] = Piece();
    // note that the piece that is in To position has moved
    __data.PiecePlacement[To.row][To.col].move();

    if (ChangeActiveColor)
    {
        // change colors to the next color to play
        __data.ActiveColor = __nextColorToPlay();
    }

    return;
}

void Board::__NO_SAFETY__castleKingSide(PieceColor Color)
{
    Board &board = *this;
    assert(board.__data.KingSquare[static_cast<size_t>(Color)].valid);
    const Vec2<int> king_square = board.__data.KingSquare[static_cast<size_t>(Color)].data;
    board.__NO_SAFETY__movePiece(king_square, __get_kingSide_king_castling_square(Color), false);
    board.__NO_SAFETY__movePiece(__get_kingSide_rook_square(Color), __get_kingSide_rook_castling_square(Color), true);

    return;
}

void Board::__NO_SAFETY__castleQueenSide(PieceColor Color)
{
    Board &board = *this;
    assert(board.__data.KingSquare[static_cast<size_t>(Color)].valid);
    const Vec2<int> king_square = board.__data.KingSquare[static_cast<size_t>(Color)].data;
    board.__NO_SAFETY__movePiece(king_square, __get_queenSide_king_castling_square(Color), false);
    board.__NO_SAFETY__movePiece(__get_queenSide_rook_square(Color), __get_queenSide_rook_castling_square(Color), true);

    return;
}

void Board::__NO_SAFETY__enPassant(const Vec2<int> From)
{
    Board &board = *this;
    assert(board.__data.EnPassantTarget.valid);
    assert(board.at(From).type() == PieceEnum::Pawn);
    // delete the en-passant-ed pawn
    // TODO: make this better logically (and looking)
    auto to_delete_pawn_position = __NO_SAFETY__getPawnMarchingSquares(board.__data.EnPassantTarget.data).first;
    board.__NO_SAFETY__movePiece(From, board.__data.EnPassantTarget.data, true);
}

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/
