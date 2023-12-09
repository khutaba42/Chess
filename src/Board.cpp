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
    bool FromInBoard = this->inBoard(From);
    bool ToInBoard = this->inBoard(To);

    /*---------------------------------------------------------------*/

    // check if they are in-board positions
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
    else if (From == To)
    {
        return PieceMovementStatus::FromAndToSquareAreTheSameSquare;
    }
    // check if From has a piece in it (to move)
    else if (!this->squareOccupied(From))
    {
        return PieceMovementStatus::FromSquareNotOccupied;
    }
    // check if the piece we are moving (from) is of the active color
    else if (this->at(From).color() != __data.ActiveColor)
    {
        return PieceMovementStatus::FromHasNonActiveColorPiece;
    }
    // check if the piece we are moving to is NOT of the active color
    else if (this->at(To).color() == __data.ActiveColor)
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
         *  1. From AND To positions are inside the board bounds.
         *  2. From position in not the same position as To.
         *  3. From has a piece of the active color to move to To.
         *  4. To has NO piece of the active color (To is empty or has another color) therefore the From piece can move there
         *  5. piece follow the piece moving rules when moving to To.
         *
         * * After this point all moves should be allowed unless:
         *      1. Active Color King is in check and if the piece moved it wouldn't resolve the check.
         *      2. Moving this active color piece would cause a check for the active color king.
         *      3. En passant failure.
         *      4. Castling failure.
         */

        Board PotentialBoard = *this;
        PieceColor CurrentActiveColor = PotentialBoard.getActiveColor();
        PotentialBoard.__NO_SAFETY__movePiece(From, To);

        if (__NO_SAFETY__isKingInCheck(*this, getActiveColor(), false))
        {
            if (__NO_SAFETY__isKingInCheck(PotentialBoard, CurrentActiveColor, true))
            {
                // if king is still in check after the piece moved then the move doesn't resolve the check
                return PieceMovementStatus::IllegalMove_ActiveColorKingIsInCheck;
            }
            else // the move resolved the check
            {
                bool isPieceAtFromAPawn = (this->at(From).type() == PieceEnum::Pawn);
                this->__NO_SAFETY__movePiece(From, To);
                if (this->squareOccupied(To))
                {
                    return PieceMovementStatus::CapturedPiece;
                }
                else if (__getEnPassantTarget().valid && (__getEnPassantTarget().data == To) && isPieceAtFromAPawn)
                {
                    return PieceMovementStatus::PawnCapturedThroughEnPassant;
                }
                else {
                    return PieceMovementStatus::MovedToAnEmptySquare;
                }
            }
        }
        else{
            if (__NO_SAFETY__isKingInCheck(PotentialBoard, CurrentActiveColor, true))
            {
                // if king is still in check after the piece moved then the move doesn't resolve the check
                return PieceMovementStatus::IllegalMove_ActiveColorKingGetsChecked;
            }
            else // the move resolved the check
            {
                bool isPieceAtFromAPawn = (this->at(From).type() == PieceEnum::Pawn);
                this->__NO_SAFETY__movePiece(From, To);
                if (this->squareOccupied(To))
                {
                    return PieceMovementStatus::CapturedPiece;
                }
                else if (__getEnPassantTarget().valid && (__getEnPassantTarget().data == To) && isPieceAtFromAPawn)
                {
                    return PieceMovementStatus::PawnCapturedThroughEnPassant;
                }
                else {
                    return PieceMovementStatus::MovedToAnEmptySquare;
                }
            }
        }
    }
    return PieceMovementStatus::UnknownStatus;
}

bool Board::isKingInCheck(const Board &board, PieceColor KingColor) const
{
    if (KingColor == PieceColor::None ||
        !board.__data.KingSquare[static_cast<size_t>(KingColor)].valid)
    {
        return false;
    }
    return __NO_SAFETY__isKingInCheck(*this, KingColor, false);
}

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

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

    int Rows = globals::board::ROWS, Cols = globals::board::COLS;

    int part = 0;
    int partCounter = 0;
    Vec2<int> part1 = {0, 0};

    std::string enPassant_str;
    std::string halfMoveClock_str;
    std::string fullMoveNumber_str;

    for (const char letter : FEN)
    {
        if (letter == ' ')
        {
            part++;
            continue;
        }
        switch (part)
        {
        case 0:
            if (letter >= '1' && letter < '8')
            {
                part1.col += static_cast<int>(letter) - static_cast<int>('1') + 2;
                break;
            }
            else if (letter == '8')
            {
                break;
            }
            else if (letter == '/')
            {
                part1.row++;
                part1.col = 0;
                break;
            }

            else
            {
                switch (letter)
                {
                case PieceLetters::WhitePawn:
                    data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Pawn, PieceColor::White);
                    break;
                case PieceLetters::WhiteKnight:
                    data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Knight, PieceColor::White);
                    break;
                case PieceLetters::WhiteBishop:
                    data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Bishop, PieceColor::White);
                    break;
                case PieceLetters::WhiteRook:
                    data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Rook, PieceColor::White);
                    break;
                case PieceLetters::WhiteQueen:
                    data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Queen, PieceColor::White);
                    break;
                case PieceLetters::WhiteKing:
                    data.PieceAmount[static_cast<size_t>(PieceColor::White)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::King, PieceColor::White);
                    data.KingSquare[static_cast<size_t>(PieceColor::White)] = part1;
                    break;
                case PieceLetters::BlackPawn:
                    data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Pawn, PieceColor::Black);
                    break;
                case PieceLetters::BlackKnight:
                    data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Knight, PieceColor::Black);
                    break;
                case PieceLetters::BlackBishop:
                    data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Bishop, PieceColor::Black);
                    break;
                case PieceLetters::BlackRook:
                    data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Rook, PieceColor::Black);
                    break;
                case PieceLetters::BlackQueen:
                    data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::Queen, PieceColor::Black);
                    break;
                case PieceLetters::BlackKing:
                    data.PieceAmount[static_cast<size_t>(PieceColor::Black)] += 1;
                    data.PiecePlacement[part1.row][part1.col] = Piece(PieceEnum::King, PieceColor::Black);
                    data.KingSquare[static_cast<size_t>(PieceColor::Black)] = part1;
                    break;
                default:
                    std::cout << "Unknown format of FEN in part 0 <" << letter << ">.\n";
                    break;
                }

                part1.col++;
            }

            break;
        case 1:
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
            break;
        case 2:
            data.Castling.push_back(letter);
            break;
        case 3:
            enPassant_str.push_back(letter);
            break;
        case 4:
            halfMoveClock_str.push_back(letter);
            break;
        case 5:
            fullMoveNumber_str.push_back(letter);
            break;

        default:
            break;
        }
    }

    // en passant translation
    if (enPassant_str == "-")
    {
        __data.EnPassantTarget.valid = false;
        //__data.EnPassantTarge has garbage value;
    }
    else
    {
        Vec2<int> EnPassantSquare;
        EnPassantSquare.row = static_cast<int>(enPassant_str[0]) - static_cast<int>('a');
        EnPassantSquare.col = static_cast<int>(enPassant_str[1]) - static_cast<int>('1');
        __data.EnPassantTarget.valid = true;
        __data.EnPassantTarget.data = EnPassantSquare;
    }

    try
    {
        data.HalfMoveClock = std::stoi(halfMoveClock_str);
        data.FullMoveNumber = std::stoi(fullMoveNumber_str);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Unknown format of FEN in part 4/5:\n\tHalf move clock = " + halfMoveClock_str + "\n\tFull move number = " + fullMoveNumber_str + "\n"));
    }

    return data;
}

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

Validity<Vec2<int>> Board::__getEnPassantTarget() const
{
    return __data.EnPassantTarget;
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
            // TODO: Castling
            for (Vec2<int> dir : globals::board::ALL_DIRECTIONS)
            {
                Vec2<int> Dest = From + dir;

                if (this->inBoard(Dest) && this->at(Dest).color() != Color)
                {
                    Board ExperimentBoard = *this;
                    ExperimentBoard.__NO_SAFETY__movePiece(From, Dest);
                    if (!__NO_SAFETY__isKingInCheck(ExperimentBoard, Color, false))
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
            }

            // // little bit ugly looking here for the king but it checks for castling pretty efficiently
            // do // scoping the variables
            // {
            //     auto &Directions = globals::board::KING_DIRECTIONS_WITH_CASTLING_AT_1_AND_3;
            //     short Counter = 0; // to look for castling
            //     bool NoCastlingThisDirection = false;
            //     for (auto dir_it = Directions.begin(); dir_it != Directions.end(); ++dir_it)
            //     {
            //         if (NoCastlingThisDirection)
            //         {
            //             NoCastlingThisDirection = false;
            //         }
            //         else
            //         {

            //             Vec2<int> Dest = From + *dir_it;

            //             if (this->inBoard(Dest) && this->at(Dest).color() != Color)
            //             {
            //                 Board ExperimentBoard = *this;
            //                 ExperimentBoard.__NO_SAFETY__movePiece(From, Dest);
            //                 if (!__NO_SAFETY__isKingInCheck(ExperimentBoard, Color, false))
            //                 {
            //                     if (this->squareOccupied(Dest))
            //                     {
            //                         Squares.occupied.push_back(Dest);
            //                     }
            //                     else
            //                     {
            //                         Squares.empty.push_back(Dest);
            //                     }
            //                 }
            //                 /* King is IN check */ else if ((Counter == 0) || (Counter == 2))
            //                 {
            //                     /**
            //                      * if the counter lands between a direction and a castling direction (0 and 2 are the squares between
            //                      * the far squares and the initial square the king was one) then note to skip the next iteration
            //                      */
            //                     NoCastlingThisDirection = true;
            //                     /*------------------------------*/
            //                 }
            //             }
            //         }

            //         ++Counter;
            //     }
            // } while (false);

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
            else if (potentialSquareColor != PieceColor::None) // there is a piece
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
            else if (potentialSquareColor != PieceColor::None) // no piece
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

void Board::__NO_SAFETY__movePiece(const Vec2<int> From, const Vec2<int> To)
{
    // the piece that was in From is now in To.
    __data.PiecePlacement[To.row][To.col] = __data.PiecePlacement[From.row][From.col];
    // remove the piece that was in From
    __data.PiecePlacement[From.row][From.col] = Piece();
    // note that the piece that is in To position has moved
    __data.PiecePlacement[To.row][To.col].move();
    // change colors to the next color to play
    __data.ActiveColor = __nextColorToPlay();
    // // update the king square
    // if (this->at(To).type() == PieceEnum::King)
    // {
    //     __data.KingSquare[static_cast<size_t>(this->at(To).color())].data = To;
    // }
}

/*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/
