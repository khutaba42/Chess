#pragma once

#include "Graphics.h"

class Game
{
public:
    Game();
    ~Game();

private:
    Graphics __graphics;

    void gameLoop();
};
