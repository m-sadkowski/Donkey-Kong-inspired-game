#pragma once

#include<SDL.h>

#include "Defines.h"

class GameObject
{
public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, double x, double y, int timesh, int timesw, float ANGLE);
	GameObject() {};
	~GameObject();

	const SDL_Rect& getDestRect() const { return destRect; }

	void changeTex(const char* texturesheet);
	SDL_Rect getRect() { return destRect; };
	void update();
	void render();

private:
	double xpos;
	double ypos;
	int numberh;
	int numberw;
	float angle;

	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;
};
