#pragma once
#include<stdio.h>

#include<SDL.h>

#include "Player.h"
#include "Barrel.h"
#include "Defines.h"

class Game 
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void restartGame();

	void createGameObjects(SDL_Renderer* renderer);
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };
	void winOrLose(SDL_Rect* playerRect);
	bool nextGame = false;
	void spawnBarrels();
	void stopPlayer();
	void blockMovement();
	void animateBarrels();

private:
	double time = 0;
	bool isWSKeyPressed = false;
	bool isADKeyPressed = false;
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int cnt = 0;
	int endingFrame = 0;
	int direction = 0;
	bool slow = 0;
	int numOfBarrels = 0;
	int barrelSpawnAcceleration = 0;
	SDL_Surface* menu;
	SDL_Surface* screen;
};
