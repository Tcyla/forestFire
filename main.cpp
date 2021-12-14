#include<iostream>
#include "headers/Game.h"


Game* g_game = nullptr;

int main(int argc, char* args[])
{
    try
    {
	    g_game = new Game("Forest Fire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, 0);
    }catch(const char* e)
    {
        std::cout << e << std::endl;
        return 1;
    }

	while(g_game->isRunning())
	{
		g_game->handleEvents();
		g_game->render();
		g_game->update();
	}
	delete g_game;

	return 0;
}
