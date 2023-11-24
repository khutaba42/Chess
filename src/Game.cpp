
#include "globals.h"
#include "Game.h"
#include "Window.h"

Game::Game()
    : __graphics()
{
    gameLoop();
}

Game::~Game()
{
}

void Game::gameLoop()
{
    while (__graphics.handleEvents() == KeepRunning);
}
