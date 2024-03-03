#pragma once

#include<SDL.h>

#include "Map.h"
#include "Defines.h"

class Player
{
public:
	Player(const char* texturesheet, SDL_Renderer* ren, int x, int y, double timesh, double timesw);
	~Player();

	void update(const Map& map, int cnt);
	void render();
	void changeTex(const char* texturesheet);
	SDL_Rect getRect() { return destRect; };
	
	bool checkCollisionWithPlatforms(const Map& map);
	bool checkCollisionWithLadders(const Map& map);
	int checkBottomCollision(const Map& map);

	void playerMovement(const Map& map);
	void goLeft(int velocity);
	void goRight(int velocity);
	void goDown(int velocity);
	void goUp(int velocity);
	int getX() { return xpos; };
	void jump(const Map& map);
	void end(bool win);
	void goBack(int x);

	int speed;
	int climb;
	int gravity = 4;

	bool isJumping = false;
	bool falling = false;

private:
	int xpos;
	int ypos;
	double width;
	double height;
	int jumpStart;

	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;
};
