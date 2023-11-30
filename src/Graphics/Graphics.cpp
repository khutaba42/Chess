#include <algorithm> // for std::min

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

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

Graphics::Graphics()
    : __window(globals::window::NAME + " " + pickRandomChessPieceSymbol()),
      __board(),
      __boardData()
{
    __updateGraphicalBoardData();
}

Graphics::~Graphics()
{
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

GameStatus Graphics::handleEvents()
{
    // Event handler
    SDL_Event event;
    // Handle events on queue
    while (SDL_WaitEvent(&event)) // SDL_WaitEvent doesn't eat up the CPU for no reason (for this game) like SDL_PollEvent do.
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
                __updateWindow(newWidth, newHeight);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            __handleMouseDownEvent(event.motion.x, event.motion.y);
            break;
        case SDL_MOUSEBUTTONUP:
            __handleMouseUpEvent(event.motion.x, event.motion.y);
            break;
        case SDL_MOUSEMOTION:
            __handleMousePosition(event.motion.x, event.motion.y);
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
        __renderGame();
    }
    return KeepRunning;
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

Graphics::GraphicalBoard &Graphics::__getGraphicalBoardData()
{
    return __boardData;
}

Graphics::GraphicalBoard::SquareData &Graphics::__getSquareDataAt(int row, int col)
{
    return __boardData.squareData[row * __board.getCols() + col];
}

int Graphics::__getWindowWidth() const
{
    return __window.getWidth();
}

int Graphics::__getWindowHeight() const
{
    return __window.getHeight();
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

void Graphics::__renderGame()
{
    // set a color to flush the back renderer
    __window.setDrawColor(globals::color::darkGreen);
    __window.clearRenderer();

    // draw the board
    __drawBoard();

    // show back renderer
    __window.presentRenderer();
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

void Graphics::__updateWindow(int Width, int Height)
{
    __window.updateWindow(Width, Height);
    __updateGraphicalBoardData();
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

void Graphics::__updateGraphicalBoardData()
{
    int WindowWidth = __getWindowWidth();
    int WindowHeight = __getWindowHeight();

    int Rows = __board.getRows();
    int Cols = __board.getCols();

    __boardData.boardFrame = {
        (WindowWidth > WindowHeight) ? ((WindowWidth - WindowHeight) / 2) : (0),
        (WindowWidth < WindowHeight) ? ((WindowHeight - WindowWidth) / 2) : (0),
        std::min(WindowWidth, WindowHeight),
        std::min(WindowWidth, WindowHeight)

    };

    __boardData.squareSideLength = __boardData.boardFrame.w / (Cols + 1);

    __boardData.boardBorders = {
        __boardData.boardFrame.x + (__boardData.boardFrame.w - (__boardData.squareSideLength) * Cols) / 2,
        __boardData.boardFrame.y + (__boardData.boardFrame.h - (__boardData.squareSideLength) * Rows) / 2,
        (__boardData.boardFrame.w / (Cols + 1)) * Cols,
        (__boardData.boardFrame.h / (Rows + 1)) * Rows

    };

    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            Rectangle coor = {
                __boardData.boardBorders.x + __boardData.squareSideLength * col,
                __boardData.boardBorders.y + __boardData.squareSideLength * row,
                __boardData.squareSideLength,
                __boardData.squareSideLength

            };
            __boardData.squareData.push_back(GraphicalBoard::SquareData(coor, {row, col}));
        }
    }
}

void Graphics::__drawBoard()
{
    auto BoardFrameColor = globals::color::darkTealGreen;
    int Rows = __board.getRows();
    int Cols = __board.getCols();

    __window.drawRectangle(
        __boardData.boardFrame.x,
        __boardData.boardFrame.y,
        __boardData.boardFrame.w,
        __boardData.boardFrame.h,
        BoardFrameColor

    );

    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            GraphicalBoard::SquareData &info = __getSquareDataAt(row, col);

            // draw "white" square
            if ((row + col) % 2 == 0)
            {
                if (info.hoveredOn)
                {
                    __window.drawRectangle(info.coordinates, globals::color::lightWhiteClicked);
                }
                else
                {
                    __window.drawRectangle(info.coordinates, globals::color::lightWhite);
                }
            }
            // draw "black" square
            else // (row + col) % 2 == 1
            {
                if (info.hoveredOn)
                {
                    __window.drawRectangle(info.coordinates, globals::color::darkGreenClicked);
                }
                else
                {
                    __window.drawRectangle(info.coordinates, globals::color::darkGreen);
                }
            }

            // Draw pieces
            if (__board.squareOccupied(info.position))
            {
                const Piece &piece = __board[info.position];
                Rectangle rect = info.coordinates;
                Rectangle dest = {rect.x, rect.y, rect.w, rect.h};
                // don't draw clicked piece
                if (__pieceIsClicked(info.position))
                {
                    __window.drawPiece(piece, &dest);
                }
                // draw clicked piece
                else
                {
                    __window.drawPiece(piece, &dest);
                }
            }
        }
    }
    // must draw the clicked piece last so it wouldn't overlap with other objects being rendered
    if (__boardData.clickedPiece.clicked)
    {
        __window.drawPiece(__board.at(__boardData.clickedPiece.initalPosition), &__boardData.clickedPiece.rect);
    }

    return;
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

bool Graphics::__pieceIsClicked(Vec2<int> Position)
{
    return (Position == __boardData.clickedPiece.initalPosition) && __boardData.clickedPiece.clicked;
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

/*
 * *---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * if the mouse is inside the board borders then we want to illuminate the square we are hovering
 * over (by setting it to illuminated in the `__boardData`)
 * *---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * if the mouse in inside the board frame and there is a piece clicked, then we want to move the piece
 * inside the board frame to follow the mouse, if the mouse got out we return the
 * piece to its original square.
 * *---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 */

void Graphics::__handleMousePosition(int x, int y)
{
    GraphicalBoard &board = __getGraphicalBoardData();

    const Rectangle &board_borders = board.boardBorders;
    const Rectangle &board_frame = board.boardFrame;
    const int side_length = board.squareSideLength;
    // calculate square position
    const int square_col = (x - board_borders.x) / side_length;
    const int square_row = (y - board_borders.y) / side_length;
    if (board_borders.inRectangle(x, y)) // if the mouse is inside the board boarders
    {

        __updateClickedPieceRectanglePosition(x, y);

        __getSquareDataAt(square_row, square_col).hoveredOn = true;
    }
    else
    {
        __getSquareDataAt(square_row, square_col).hoveredOn = false;
        if (board_frame.inRectangle(x, y)) // if the mouse is inside the board frame
        {
            __updateClickedPieceRectanglePosition(x, y);
        }
    }
    return;
}

void Graphics::__handleMouseDownEvent(int x, int y)
{
    __clickOnPiece(x, y);
}

void Graphics::__handleMouseUpEvent(int x, int y)
{
    __releasePiece(x, y);
}

void Graphics::__clickOnPiece(int x, int y)
{
    const Rectangle &board_frame = __boardData.boardFrame;
    const Rectangle &board_borders = __boardData.boardBorders;
    const int side_length = __boardData.squareSideLength;
    const int square_col = (x - board_borders.x) / side_length;
    const int square_row = (y - board_borders.y) / side_length;

    if (!__boardData.clickedPiece.clicked)
    {
        __boardData.clickedPiece.clicked = true;
        __boardData.clickedPiece.initalPosition = {square_row, square_col};
        __boardData.clickedPiece.rect = Rectangle::createRectangleFromMiddle(x, y, side_length, side_length);
    }
}

void Graphics::__releasePiece(int x, int y)
{
    if (__boardData.clickedPiece.clicked)
    {
        __boardData.clickedPiece.clicked = false;

        const Rectangle &board_frame = __boardData.boardFrame;
        const Rectangle &board_borders = __boardData.boardBorders;
        const int side_length = __boardData.squareSideLength;
        const int square_col = (x - board_borders.x) / side_length;
        const int square_row = (y - board_borders.y) / side_length;
        if (board_borders.inRectangle(x, y))
        {
            Vec2<int> square_coor = {square_row, square_col};
            if (__board.inBoard(square_coor))
            {
                __board.swapPieces(__boardData.clickedPiece.initalPosition, square_coor);
            }
        }
    }
}

void Graphics::__updateClickedPieceRectanglePosition(int x, int y)
{
    if (__boardData.clickedPiece.clicked)
    {
        const int side_length = __boardData.squareSideLength;
        __boardData.clickedPiece.rect = Rectangle::createRectangleFromMiddle(x, y, side_length, side_length);
    }
}