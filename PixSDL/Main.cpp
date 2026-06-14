#include "Game.h"

int main(int argc, char* argv[])
{
    pix::Game* game = new pix::Game("CompanyName", "Game", pix::LaunchConfigData());

    game->Run();

    delete game;

    return 0;
}