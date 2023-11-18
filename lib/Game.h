#pragma once

#include "includes.h"
#include "Window.h"
#include "Piece.h"
#include "Board.h"

class Game
{
public:
    Game();
    ~Game();

private:
    Window __window;
    std::map<PieceEnum, SDL_Texture *> __pieceTextures;
    Board __board;

    void gameLoop();

    void drawBoard();

    void loadPieceTextures();

};
