#include "Board.h"

Board::Board(const std::string &FEN)
    : __data(FEN_decoder(FEN)),
      __rows(globals::board::ROWS),
      __cols(globals::board::COLS)
{
}

Board::~Board()
{
    iterateRowCol(
        [this](int row, int col)
        {
            delete this->__data.PiecePlacement[row][col];
        }

    );
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
    return inBoard(position) && (__data.PiecePlacement[position.row][position.col] != nullptr);
}

const Piece Board::operator[](const Vec2<int> position) const
{
    if (squareOccupied(position))
    {
        return *__data.PiecePlacement[position.row][position.col];
    }
    else
    {
        return Piece();
    }
}

const Piece &Board::at(const Vec2<int> position) const
{
    if (squareOccupied(position))
    {
        return *__data.PiecePlacement[position.row][position.col];
    }
    else
    {
        return __data.EmptyPiece;
    }
}

void Board::swapPieces(const Vec2<int> a, const Vec2<int> b)
{
    if (inBoard(a) && inBoard(b))
    {
        std::swap(__data.PiecePlacement[a.row][a.col], __data.PiecePlacement[b.row][b.col]);
    }
    return;
}

Board::PieceMovementStatus Board::movePiece(const Vec2<int> From, const Vec2<int> To)
{
    //? Variables for the Do Nothing statements
    bool FromInBoard = this->inBoard(From);
    bool ToInBoard = this->inBoard(To);

    bool FromSquareOccupied = this->squareOccupied(From);
    //? Variables for the Mixed statements

    //? Variables for the Do Something statements

    /*---------------------------------------------------------------*/

    /**
     *    * Do Nothing statements,
     *  returns the reason the board remains unchanged
     */
    // check if they are in-board positions
    if (!FromInBoard || !ToInBoard)
    {
        if (FromInBoard && !ToInBoard)
        {
            return ToPositionNotInBoard;
        }
        else if (!FromInBoard && ToInBoard)
        {
            return FromPositionNotInBoard;
        }
        else // if(!FromInBoard && !ToInBoard)
        {
            return FromAndToPositionNotInBoard;
        }
    }
    // check if From has a piece in it (to move)
    else if (!FromSquareOccupied)
    {
        return FromSquareNotOccupied;
    }
    // check if the piece we are moving (from) is of the active color
    else if (this->at(From).color() != __data.ActiveColor)
    {
        return FromHasNonActiveColorPiece;
    }

    // check if the move in En passant
    // else if ()
    // {
    //     return ;
    // }
    /**
     *    * Mixed statements,
     *  returns the reason the board changed after the change has been done
     *  or
     *  the reason the board remained unchanged.
     */

    // check if the piece we are moving to is NOT of the active color
    else if (this->at(To).color() == __data.ActiveColor)
    {
        return ToHasActiveColorPiece;
    }
    /**
     *    * Do Something statements,
     *  returns the reason the board changed after the change has been done.
     */
    return UnknownStatus;
}

bool Board::isKingInCheck(const Board &board, PieceColor KingColor)
{
    if (KingColor == PieceColor::None)
    {
        return false;
    }
    PieceColor activeColor = board.getActiveColor();

    // Look for the king square
}

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
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            data.PiecePlacement[row][col] = nullptr;
        }
    }

    int part = 0;
    int partCounter = 0;
    Vec2<int> part1 = {0, 0};

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
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Pawn, PieceColor::White);
                    break;
                case PieceLetters::WhiteKnight:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Knight, PieceColor::White);
                    break;
                case PieceLetters::WhiteBishop:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Bishop, PieceColor::White);
                    break;
                case PieceLetters::WhiteRook:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Rook, PieceColor::White);
                    break;
                case PieceLetters::WhiteQueen:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Queen, PieceColor::White);
                    break;
                case PieceLetters::WhiteKing:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::King, PieceColor::White);
                    break;
                case PieceLetters::BlackPawn:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Pawn, PieceColor::Black);
                    break;
                case PieceLetters::BlackKnight:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Knight, PieceColor::Black);
                    break;
                case PieceLetters::BlackBishop:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Bishop, PieceColor::Black);
                    break;
                case PieceLetters::BlackRook:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Rook, PieceColor::Black);
                    break;
                case PieceLetters::BlackQueen:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::Queen, PieceColor::Black);
                    break;
                case PieceLetters::BlackKing:
                    data.PiecePlacement[part1.row][part1.col] = new Piece(PieceEnum::King, PieceColor::Black);
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
            data.EnPassantTarget.push_back(letter);
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

    try
    {
        data.HalfMoveClock = std::stoi(halfMoveClock_str);
        data.FullMoveNumber = std::stoi(fullMoveNumber_str);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Unknown format of FEN in part 4/5:\n\tHalf move clock = " + halfMoveClock_str + "\n\tFull move number = " + fullMoveNumber_str + "\n");
    }

    return data;
}