#include "Graphics.h"
#include "globals.h"
#include "Rectangle.h"

//-----------------//-----------------//-----------------//-----------------//

#include <random> // for the window name
std::string pickRandomChessPieceSymbol()
{ // Create a random number generator engine
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define a distribution for selecting an index
    std::uniform_int_distribution<size_t> distribution(0, globals::piece::chessPieces.size() - 1);

    // Generate a random index
    size_t random_index = distribution(gen);

    // Return the randomly selected chess piece symbol
    return globals::piece::chessPieces[random_index];
}

//-----------------//-----------------//-----------------//-----------------//

Graphics::Graphics()
    : __window(globals::window::NAME + " " + pickRandomChessPieceSymbol()),
      __board(globals::board::FEN_default),
      __rectangles(*this, __board),
      __piecesTexture(loadPieceTexture(globals::piece::spritePath))
{
}

Graphics::~Graphics()
{
    destroyTexture(__piecesTexture);
}

GameStatus Graphics::handleEvents()
{
    // Event handler
    SDL_Event event;
    // Handle events on queue
    while (SDL_WaitEvent(&event)) // TODO: why doesn't SDL_WaitEvent work?
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return QuitGame;
            break;
        case SDL_WINDOWEVENT:

            if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                int newWidth = event.window.data1;
                int newHeight = event.window.data2;
                this->updateWindow(newWidth, newHeight);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            std::cout << "mouse clicked at (" << event.motion.x << "," << event.motion.y << ")\n";
            break;
        case SDL_MOUSEBUTTONUP:
            std::cout << "mouse released at (" << event.motion.x << "," << event.motion.y << ")\n";
            break;
        case SDL_MOUSEMOTION:
            handleMousePosition(event.motion.x, event.motion.y);
            //  std::cout << "mouse at (" << event.motion.x << "," << event.motion.y << ")\n";
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return QuitGame;
            break;
        default:
            break;
        }

        // Rendering

        SDL_SetRenderDrawColor(
            this->getRenderer(),
            globals::color::black.red(),
            globals::color::black.green(),
            globals::color::black.blue(),
            globals::color::black.opacity()

        );

        SDL_RenderClear(this->getRenderer());

        this->drawBoard();

        SDL_RenderPresent(this->getRenderer());
    }
    return KeepRunning;
}

int Graphics::getWindowWidth() const { return __window.getWidth(); }
int Graphics::getWindowHeight() const { return __window.getHeight(); }
SDL_Renderer *Graphics::getRenderer() { return __window.getRenderer(); }

#include <utility> // for std::pair
#include <map>

const int SPRITE_PIECE_SIDE_LENGTH_VER_1 = 133;
std::map<std::pair<PieceEnum, PieceColor>, Rectangle> pieceRectanglesVER1 = {
    // Pieces
    {   {PieceEnum::King    , PieceColor::Black },  {0  , 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Queen   , PieceColor::Black },  {133, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Bishop  , PieceColor::Black },  {266, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Knight  , PieceColor::Black },  {399, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Rook    , PieceColor::Black },  {532, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Pawn    , PieceColor::Black },  {665, 133, SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::King    , PieceColor::White },  {0  , 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Queen   , PieceColor::White },  {133, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Bishop  , PieceColor::White },  {266, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Knight  , PieceColor::White },  {399, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Rook    , PieceColor::White },  {532, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},
    {   {PieceEnum::Pawn    , PieceColor::White },  {665, 0  , SPRITE_PIECE_SIDE_LENGTH_VER_1, SPRITE_PIECE_SIDE_LENGTH_VER_1}},

    // None
    {{PieceEnum::None, PieceColor::None}, {0, 0, 0, 0}}

};
const int SPRITE_PIECE_SIDE_LENGTH_VER_2 = 64;
std::map<std::pair<PieceEnum, PieceColor>, Rectangle> pieceRectanglesVER2 = {
    // Pieces
    {   {PieceEnum::King    , PieceColor::Black },  {0  , 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Queen   , PieceColor::Black },  {64 , 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Rook    , PieceColor::Black },  {128, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Knight  , PieceColor::Black },  {192, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Bishop  , PieceColor::Black },  {256, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Pawn    , PieceColor::Black },  {320, 0 , SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::King    , PieceColor::White },  {0  , 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Queen   , PieceColor::White },  {64 , 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Rook    , PieceColor::White },  {128, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Knight  , PieceColor::White },  {192, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Bishop  , PieceColor::White },  {256, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},
    {   {PieceEnum::Pawn    , PieceColor::White },  {320, 64, SPRITE_PIECE_SIDE_LENGTH_VER_2, SPRITE_PIECE_SIDE_LENGTH_VER_2}},

    // None
    {{PieceEnum::None, PieceColor::None}, {0, 0, 0, 0}}

};
std::map<std::pair<PieceEnum, PieceColor>, Rectangle> pieceRectangles = pieceRectanglesVER2;

void Graphics::drawBoard()
{
    int Rows = __board.getRows();
    int Cols = __board.getCols();

    __window.drawRectangle(
        __rectangles.BoardFrame.x,
        __rectangles.BoardFrame.y,
        __rectangles.BoardFrame.w,
        __rectangles.BoardFrame.h,
        globals::color::darkTealGreen

    );

    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            Rectangle rect = __rectangles.Squares[row * Cols + col];
            Vec2<int> square_coor(row, col);

            if ((row + col) % 2 == 0)
            {
                if (__rectangles.IlluminatedSquare.first && (__rectangles.IlluminatedSquare.second == square_coor))
                {
                    __window.drawRectangle(rect, globals::color::lightWhiteClicked);
                }
                else
                {
                    __window.drawRectangle(rect, globals::color::lightWhite);
                }
            }
            else
            {
                if (__rectangles.IlluminatedSquare.first && (__rectangles.IlluminatedSquare.second == square_coor))
                {
                    __window.drawRectangle(rect, globals::color::darkGreenClicked);
                }
                else
                {
                    __window.drawRectangle(rect, globals::color::darkGreen);
                }
            }

            if (__board.inBoard(square_coor) && (__board.squareOccupied(square_coor)))
            {
                const Piece &piece = __board[square_coor];
                Rectangle dest = {rect.x, rect.y, rect.w, rect.h};
                __window.drawTexture(__piecesTexture, &pieceRectangles.at({piece.type(), piece.color()}), &dest);
            }
        }
    }
    return;
}

#include <iostream>
void Graphics::handleMousePosition(int x, int y)
{
    const Rectangle &board_borders = __rectangles.BoardBorders;
    const int side_length = __rectangles.SquareSideLength;
    const int square_col = (x - board_borders.x) / side_length;
    const int square_row = (y - board_borders.y) / side_length;

    if (board_borders.inRectangle(x, y))
    {

        Vec2<int> square_coor = {square_row, square_col};
        if (__board.inBoard(square_coor))
        {
            __rectangles.IlluminatedSquare = {true, square_coor};
        }
    }
    else
    {
        __rectangles.IlluminatedSquare.first = false;
    }
    return;
}

SDL_Texture *Graphics::loadPieceTexture(const std::string &Path)
{
    // load pieces
    SDL_Surface *surface = nullptr;
    surface = SDL_LoadBMP(Path.c_str());
    if (!surface)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to create surface for image " + Path + "\n");
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(this->getRenderer(), surface);
    if (!texture)
    {
        printf("Unable to load bitmap texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        throw std::runtime_error("Failed to create texture.\n");
    }
    SDL_FreeSurface(surface);

    return texture;
}

void Graphics::destroyTexture(SDL_Texture *&Texture)
{
    SDL_DestroyTexture(Texture);
    Texture = nullptr;
}

Graphics::Rect_data::Rect_data(const Graphics &graphics, const Board &board)
    : BoardFrame({

          (graphics.getWindowWidth() > graphics.getWindowHeight()) ? ((graphics.getWindowWidth() - graphics.getWindowHeight()) / 2) : (0),
          (graphics.getWindowWidth() < graphics.getWindowHeight()) ? ((graphics.getWindowHeight() - graphics.getWindowWidth()) / 2) : (0),
          std::min(graphics.getWindowWidth(), graphics.getWindowHeight()),
          std::min(graphics.getWindowWidth(), graphics.getWindowHeight())

      }),
      SquareSideLength(BoardFrame.w / (board.getCols() + 1)),
      BoardBorders({

          BoardFrame.x + (BoardFrame.w - (SquareSideLength)*board.getCols()) / 2,
          BoardFrame.y + (BoardFrame.h - (SquareSideLength)*board.getRows()) / 2,
          (BoardFrame.w / (board.getCols() + 1)) * board.getCols(),
          (BoardFrame.h / (board.getRows() + 1)) * board.getRows()

      }),
      Squares(),
      IlluminatedSquare({false, {0, 0}})
{

    int Cols = board.getCols();
    int rectSideLength = BoardBorders.w / Cols;
    board.iterateRowCol(

        [rectSideLength, Cols, this](int row, int col)
        {
            Squares.push_back({this->BoardBorders.x + rectSideLength * col,
                               this->BoardBorders.y + rectSideLength * row,
                               rectSideLength,
                               rectSideLength

            });
        }

    );
}

void Graphics::updateWindow(int Width, int Height)
{
    __window.updateWindow(Width, Height);
    this->updateRectData();
}

void Graphics::updateRectData()
{
    __rectangles = Rect_data(*this, this->__board);
}
