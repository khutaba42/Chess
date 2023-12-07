#include "Exceptions.h"
#include "Game.h"

int main(int argc, char *argv[])
{
    try
    {
        Game game;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
