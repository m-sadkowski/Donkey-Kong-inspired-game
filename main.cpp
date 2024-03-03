#include<SDL.h>

#include<cstdlib>
#include<stdio.h>

#include "Game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
const int TARGET_FPS = 60;
const int DELAY_TIME = 1000 / TARGET_FPS;

// MAIN

int main(int argc, char* argv[]) {

    Game* game = new Game();

    game->init("King Donkey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
	SDL_ShowCursor(SDL_DISABLE);

    while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
		SDL_Delay(DELAY_TIME);
		if (game->nextGame)
		{
			game->nextGame = false;
			game->restartGame();
		}
	}

	SDL_Delay(1000);

	game->clean();

    return 0;
}
