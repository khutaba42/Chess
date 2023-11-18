#include "Board.h"

Board::Board(const std::string &FEN)
{
    for (int row = 0; row < globals::Board::ROWS; row++)
    {
        for (int col = 0; col < globals::Board::COLS; col++)
        {
            __piecePlacement[row][col] = nullptr;
        }
    }

    FEN_decoder(FEN);
}

Board::~Board()
{
    for (int row = 0; row < globals::Board::ROWS; row++)
    {
        for (int col = 0; col < globals::Board::COLS; col++)
        {
            delete __piecePlacement[row][col];
        }
    }
}

bool Board::isEmpty(const Vec2<int> position) const
{
    if (!(position.row < 0 || position.row >= globals::Board::ROWS || position.col < 0 || position.col >= globals::Board::COLS))
    {
        return (__piecePlacement[position.row][position.col] == nullptr);
    }
    throw std::runtime_error("Invalid position in board in isEmpty().\n");
}

const Piece Board::operator[](const Vec2<int> position) const
{
    if (!(position.row < 0 || position.row >= globals::Board::ROWS || position.col < 0 || position.col >= globals::Board::COLS))
    {
        if (__piecePlacement[position.row][position.col] != nullptr)
        {
            return *__piecePlacement[position.row][position.col];
        }
        else
        {
            return Piece();
        }
    }
    throw std::runtime_error("Invalid position in board in operator[].\n");
}

void Board::FEN_decoder(const std::string &FEN)
{
    /**
     * 0 - Piece placement
     * 1 - Active color
     * 2 - Castling availability
     * 3 - En passant target square
     * 4 - Halfmove clock
     * 5 - Fullmove number
     */
    int part = 0;
    int partCounter = 0;
    Vec2<int> part1 = {0, 0};

    std::string HalfmoveClock_str;
    std::string FullmoveNumber_str;

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
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::WhitePawn);
                    break;
                case 'N':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::WhiteKnight);
                    break;
                case 'B':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::WhiteBishop);
                    break;
                case 'R':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::WhiteRook);
                    break;
                case 'Q':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::WhiteQueen);
                    break;
                case 'K':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::WhiteKing);
                    break;
                // handle black
                case 'p':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::BlackPawn);
                    break;
                case 'n':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::BlackKnight);
                    break;
                case 'b':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::BlackBishop);
                    break;
                case 'r':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::BlackRook);
                    break;
                case 'q':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::BlackQueen);
                    break;
                case 'k':
                    __piecePlacement[part1.row][part1.col] = new Piece(PieceEnum::BlackKing);
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
                __activeColor = PieceColor::White;
            }
            else if (letter == 'b')
            {
                __activeColor = PieceColor::Black;
            }
            else
            {
                std::cout << "Unknown format of FEN in part 1 <" << letter << ">.\n";
            }
            break;
        case 2:
            __castling.push_back(letter);
            break;
        case 3:
            __EnPassantTarget.push_back(letter);
            break;
        case 4:
            HalfmoveClock_str.push_back(letter);
            break;
        case 5:
            FullmoveNumber_str.push_back(letter);
            break;

        default:
            break;
        }
    }

    try
    {
        __HalfmoveClock = std::stoi(HalfmoveClock_str);
        __FullmoveNumber = std::stoi(FullmoveNumber_str);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Unknown format of FEN in part 4/5:\n\tHalf move clock = " + HalfmoveClock_str + "\n\tFull move number = " + FullmoveNumber_str + "\n");
    }
}
