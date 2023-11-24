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
        });
}

int Board::getRows() const { return __rows; }
int Board::getCols() const { return __cols; }

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

Board::FEN_data Board::FEN_decoder(const std::string &FEN)
{
    /**
     * 0 - Piece placement
     * 1 - Active color
     * 2 - Castling availability
     * 3 - En passant target square
     * 4 - Halfmove clock
     * 5 - Fullmove number
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

    data.EmptyPiece = Piece();
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
                // handle white
                case 'P':
                    data.PiecePlacement[part1.row][part1.col] = new Pawn(PieceColor::White);
                    break;
                case 'N':
                    data.PiecePlacement[part1.row][part1.col] = new Knight(PieceColor::White);
                    break;
                case 'B':
                    data.PiecePlacement[part1.row][part1.col] = new Bishop(PieceColor::White);
                    break;
                case 'R':
                    data.PiecePlacement[part1.row][part1.col] = new Rook(PieceColor::White);
                    break;
                case 'Q':
                    data.PiecePlacement[part1.row][part1.col] = new Queen(PieceColor::White);
                    break;
                case 'K':
                    data.PiecePlacement[part1.row][part1.col] = new King(PieceColor::White);
                    break;
                // handle black
                case 'p':
                    data.PiecePlacement[part1.row][part1.col] = new Pawn(PieceColor::Black);
                    break;
                case 'n':
                    data.PiecePlacement[part1.row][part1.col] = new Knight(PieceColor::Black);
                    break;
                case 'b':
                    data.PiecePlacement[part1.row][part1.col] = new Bishop(PieceColor::Black);
                    break;
                case 'r':
                    data.PiecePlacement[part1.row][part1.col] = new Rook(PieceColor::Black);
                    break;
                case 'q':
                    data.PiecePlacement[part1.row][part1.col] = new Queen(PieceColor::Black);
                    break;
                case 'k':
                    data.PiecePlacement[part1.row][part1.col] = new King(PieceColor::Black);
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