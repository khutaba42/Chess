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

//-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*/

Graphics::GraphicalBoard &Graphics::__getGraphicalBoardData()
{
    return __boardData;
}

Graphics::GraphicalBoard::SquareData &Graphics::__getSquareDataAt(Vec2<int> Pos)
{
    return __boardData.squareData[Pos.row * __board.getCols() + Pos.col];
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
    __window.setDrawColor(globals::color::black);
    __window.clearRenderer();

    // draw the board
    __drawBoard(true);

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

    const int boardFrameWidthAndHeight = std::min(WindowWidth, WindowHeight);

    int Rows = __board.getRows();
    int Cols = __board.getCols();

    __boardData.boardFrame = {
        (WindowWidth > WindowHeight) ? ((WindowWidth - WindowHeight) / 2) : (0),
        (WindowWidth < WindowHeight) ? ((WindowHeight - WindowWidth) / 2) : (0),
        boardFrameWidthAndHeight,
        boardFrameWidthAndHeight

    };

    __boardData.squareSideLength = __boardData.boardFrame.w / (Cols + 1);

    const int boardBoardersWidth = (__boardData.boardFrame.w / (Cols + 1)) * Cols;
    const int boardBoardersHeight = (__boardData.boardFrame.h / (Rows + 1)) * Rows;

    __boardData.boardBorders = {
        __boardData.boardFrame.x + ((__boardData.boardFrame.w - boardBoardersWidth) / 2),
        __boardData.boardFrame.y + ((__boardData.boardFrame.h - boardBoardersHeight) / 2),
        boardBoardersWidth,
        boardBoardersHeight

    };
    /**
     * ! this following line is Important
     * it prevents a bug where the vector `squareData` keeps growing
     * and the board only shows the first ever squares on the renderer
     */
    __boardData.squareData.clear();

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

void Graphics::__drawBoard(bool flipped)
{
    auto BoardFrameColor = globals::color::darkTealGreen;
    int Rows = __board.getRows();
    int Cols = __board.getCols();

    __window.drawRectangle(
        __boardData.boardFrame,
        BoardFrameColor

    );

    GraphicalBoard &boardData = __getGraphicalBoardData();

    // draw base color "light square" first
    __window.drawRectangle(boardData.boardBorders, globals::color::lightWhite);

    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            GraphicalBoard::SquareData &info = __getSquareDataAt(row, col);

            // draw "dark squares" on top
            if ((row + col) % 2 == 1)
            {
                __window.drawRectangle(info.coordinates, globals::color::darkGreen);
            }
        }
    }

    Validity<Vec2<int>> pos = __getHoveredOnPosition();
    if (pos.valid)
    {
        GraphicalBoard::SquareData &info = __getSquareDataAt(pos.data);
        if ((pos.data.row + pos.data.col) % 2 == 0)
        {
            __window.drawRectangle(info.coordinates, globals::color::lightWhiteClicked);
        }
        else // if ((row + col) % 2 == 1)
        {
            __window.drawRectangle(info.coordinates, globals::color::darkGreenClicked);
        }
    }

    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            GraphicalBoard::SquareData &info = __getSquareDataAt(row, col);
            // Draw pieces
            if (__board.squareOccupied(info.position))
            {
                const Piece &piece = __board[info.position];
                Rectangle rect = info.coordinates;
                Rectangle dest = {rect.x, rect.y, rect.w, rect.h};
                // don't draw clicked piece
                if (!__pieceIsClicked(info.position))
                {
                    __window.drawPiece(piece, &dest);
                }
            }
        }
    }

    // draw the clicked piece
    if (__boardData.mouseInfo.clickedPieceInitalPosition.valid)
    {
        Vec2<int> clickedPiecePos = __boardData.mouseInfo.clickedPieceInitalPosition.data;
        // draw circles to specify squares that are attacked by the clicked piece
        Board::AttackedSquares attackedPositions = __board.getAllSquaresPieceCanGoTo(clickedPiecePos);
        for (Vec2<int> Position : attackedPositions.empty)
        {
            __window.drawAttackedSquareCircle(&__getSquareDataAt(Position).coordinates, true);
        }
        for (Vec2<int> Position : attackedPositions.occupied)
        {
            __window.drawAttackedSquareCircle(&__getSquareDataAt(Position).coordinates, false);
        }
        // must draw the clicked piece last so it wouldn't overlap with other objects being rendered
        __window.drawPiece(__board.at(clickedPiecePos), &__boardData.mouseInfo.clickedPieceRectangle);
    }

    return;
}

// TODO: fix flip board bugs
void Graphics::__flipBoard()
{
    (__boardData.BoardIsFlipped) ? (__boardData.BoardIsFlipped = false) : (__boardData.BoardIsFlipped = true); // indicate that the board is flipped
    bool flipped = __boardData.BoardIsFlipped;

    // flip the positions in the square data
    int Rows = __board.getRows();
    int Cols = __board.getCols();
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            if (flipped)
            {
                __boardData.squareData[row * Cols + col].position = {Rows - 1 - row, Cols - 1 - col};
            }
            else
            {
                __boardData.squareData[row * Cols + col].position = {row, col};
            }
        }
    }
}

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--- ///

Validity<Vec2<int>> Graphics::__getHoveredOnPosition() const
{
    return __boardData.mouseInfo.hoveredOnPosition;
}

Validity<Vec2<int>> Graphics::__getClickedOnPosition() const
{
    return __boardData.mouseInfo.clickedPieceInitalPosition;
}

Rectangle Graphics::__getHeldPieceRectangle() const
{
    return __boardData.mouseInfo.clickedPieceRectangle;
}

bool Graphics::__squareIsHoveredOn(Vec2<int> Position)
{
    return (__boardData.mouseInfo.hoveredOnPosition.valid) && (Position == __boardData.mouseInfo.hoveredOnPosition.data);
}

bool Graphics::__pieceIsClicked(Vec2<int> Position)
{
    return (__boardData.mouseInfo.clickedPieceInitalPosition.valid) && (Position == __boardData.mouseInfo.clickedPieceInitalPosition.data);
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

        board.mouseInfo.hoveredOnPosition.valid = true;
        board.mouseInfo.hoveredOnPosition.data = {square_row, square_col};
    }
    else
    {
        board.mouseInfo.hoveredOnPosition.valid = false;

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

    if (!__boardData.mouseInfo.clickedPieceInitalPosition.valid && board_borders.inRectangle(x, y))
    {
        __boardData.mouseInfo.clickedPieceInitalPosition.valid = true;
        __boardData.mouseInfo.clickedPieceInitalPosition.data = {square_row, square_col};
        __boardData.mouseInfo.clickedPieceRectangle = Rectangle::createRectangleFromMiddle(x, y, side_length, side_length);
    }
}

void Graphics::__releasePiece(int x, int y)
{
    if (__boardData.mouseInfo.clickedPieceInitalPosition.valid)
    {
        const Rectangle &board_frame = __boardData.boardFrame;
        const Rectangle &board_borders = __boardData.boardBorders;
        const int side_length = __boardData.squareSideLength;
        const int square_col = (x - board_borders.x) / side_length;
        const int square_row = (y - board_borders.y) / side_length;
        if (board_borders.inRectangle(x, y))
        {
            Vec2<int> square_coor = {square_row, square_col};
            // if (__board.inBoard(square_coor))
            // {
            //     __board.swapPieces(__boardData.mouseInfo.clickedPieceInitalPosition.data, square_coor);
            // }
            __board.movePiece(__boardData.mouseInfo.clickedPieceInitalPosition.data, square_coor);
        }

        __boardData.mouseInfo.clickedPieceInitalPosition.valid = false;
    }
}

void Graphics::__updateClickedPieceRectanglePosition(int x, int y)
{
    if (__boardData.mouseInfo.clickedPieceInitalPosition.valid)
    {
        const int side_length = __boardData.squareSideLength;
        __boardData.mouseInfo.clickedPieceRectangle = Rectangle::createRectangleFromMiddle(x, y, side_length, side_length);
    }
}