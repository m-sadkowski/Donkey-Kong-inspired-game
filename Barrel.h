#pragma once

#include<SDL.h>

#include "Map.h"	
#include "Defines.h"

class Barrel
{
public:
	Barrel(const char* texturesheet, SDL_Renderer* ren, int x, int y, int timesh, int timesw);
	~Barrel();

	void update(const Map& map);
	void render();
	void changeTex(const char* texturesheet);
	SDL_Rect getRect() { return destRect; };

	void barrelMovement(const Map& map);
	int checkBottomCollision(const Map& map);
	void newBarrel(int x, int y);

	int speed;
	int previousSpeed;

	friend class Game;

private:
	int xpos;
	int ypos;
	int width;
	int height;
	bool falling = false;
	int gravity = GRAVITY;

	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;
};
