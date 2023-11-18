#include "includes.h"
#include "globals.h"
#include "Game.h"
#include "Window.h"

Game::Game()
    : __window("ChessBoard"),
      __pieceTextures()
{
    // local scope to delete any unnecessary variables
    {
    }
    loadPieceTextures();

    gameLoop();
}

Game::~Game()
{
    for (auto texture : __pieceTextures)
    {
        SDL_DestroyTexture(texture.second);
    }
}

void Game::gameLoop()
{
    // Main loop flag
    bool running = true;

    // While application is running
    while (running)
    {
        // Event handler
        SDL_Event event;
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    int newWidth = event.window.data1;
                    int newHeight = event.window.data2;
                    __window.setHeight(newHeight);
                    __window.setWidth(newWidth);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                std::cout << "mouse clicked at (" << event.motion.x << "," << event.motion.y << ")\n";
                break;
            case SDL_MOUSEBUTTONUP:
                std::cout << "mouse released at (" << event.motion.x << "," << event.motion.y << ")\n";
                break;
            case SDL_MOUSEMOTION:
                std::cout << "mouse at (" << event.motion.x << "," << event.motion.y << ")\n";
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
            // Add more cases for other event types if needed
            default:
                break;
            }
        }

        // Rendering
        SDL_SetRenderDrawColor(__window.getRenderer(),
                               globals::COLOR::darkTealGreen.red(),
                               globals::COLOR::darkTealGreen.green(),
                               globals::COLOR::darkTealGreen.blue(),
                               globals::COLOR::darkTealGreen.opacity());

        SDL_RenderClear(__window.getRenderer());

        drawBoard();

        SDL_RenderPresent(__window.getRenderer());
    }
}

void Game::drawBoard()
{
    int width = __window.getWidth();
    int height = __window.getHeight();

    int rectWidth = width / globals::Board::COLS;
    int rectHeight = height / globals::Board::ROWS;
    int rectSide = std::min(rectWidth, rectHeight);

    int startPixelX = (width > height) ? ((width - height) / 2) : (0);
    int startPixelY = (height > width) ? ((height - width) / 2) : (0);

    for (size_t row = 0; row < globals::Board::ROWS; row++)
    {
        for (size_t col = 0; col < globals::Board::COLS; col++)
        {
            int rectX = startPixelX + col * rectSide;
            int rectY = startPixelY + row * rectSide;
            if ((row + col) % 2 == 0)
            {
                __window.drawRectangle(rectX, rectY, rectSide, rectSide, globals::COLOR::lightWhite);
            }
            else
            {
                __window.drawRectangle(rectX, rectY, rectSide, rectSide, globals::COLOR::darkGreen);
            }

            if (!__board.isEmpty(Vec2<int>(row, col)))
            {
                const Piece& piece = __board[Vec2<int>(row, col)];
                SDL_Rect rect = {rectX, rectY, rectSide, rectSide};
                SDL_RenderCopy(__window.getRenderer(), __pieceTextures[piece.type()], nullptr, &rect);
            }
        }
    }
}

void Game::loadPieceTextures()
{
    // load pieces
    for (auto piece : globals::piece::base)
    {
        SDL_Surface *surface = SDL_LoadBMP(std::string(globals::piece::Path + piece.second + globals::piece::ext::bmp).c_str());
        if (!surface)
        {
            printf("Unable to load bitmap: %s\n", SDL_GetError());
            throw std::runtime_error("Failed to create surface for image " + globals::piece::Path + piece.second + globals::piece::ext::bmp);
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(__window.getRenderer(), surface);
        if (!texture)
        {
            printf("Unable to load bitmap texture: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            throw std::runtime_error("Failed to create texture for image " + piece.second);
        }
        __pieceTextures[piece.first] = texture;

        SDL_FreeSurface(surface);
    }
    __pieceTextures[PieceEnum::None] = nullptr;
}

