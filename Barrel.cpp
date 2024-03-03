#include"Barrel.h"
#include"TextureManager.h"
#include"Map.h"

#include<stdio.h>

const int jumpHeight = 2 * BARRELSIZE;

Barrel::Barrel(const char* texturesheet, SDL_Renderer* ren, int x, int y, int timesh, int timesw)
{
	renderer = ren;
	objTexture = TextureManager::LoadTexture(texturesheet, ren);

	xpos = x * BARRELSIZE;
	ypos = y * BARRELSIZE;
	width = timesh;
	height = timesw;
	speed = 0;
	previousSpeed = -BARRELSPEED;
}

Barrel::~Barrel() {}

void Barrel::changeTex(const char* texturesheet)
{
	SDL_DestroyTexture(objTexture);
	objTexture = TextureManager::LoadTexture(texturesheet, renderer);
}

int Barrel::checkBottomCollision(const Map& map)
{
	for (int i = 0; i < PLATFORMS; i++)
	{
		const SDL_Rect& platformRect = map.getPlatform(i).getDestRect();

		if (destRect.y + destRect.h == platformRect.y)
		{
			if ((destRect.x + destRect.w > platformRect.x && destRect.x < platformRect.x + platformRect.w))
			{
				//printf("Barrel collides with platform %d\n", i);
				return 1;
			}
		}
	}
	return 0;
}

void Barrel::barrelMovement(const Map& map)
{
	if (checkBottomCollision(map) == 0)
	{
		falling = true;
		if (speed > 0)
		{
			speed = 1;
		}
		else
		{
			speed = -1;
		}
	}
	if (falling)
	{
		if (checkBottomCollision(map) == 1)
		{
			falling = false;
			speed = -1 * previousSpeed;
			previousSpeed = speed;
		}
		gravity = GRAVITY;
	}
	if (!falling)
	{
		gravity = 0;
	}
}

void Barrel::update(const Map& map)
{
	srcRect.h = BARRELSIZE * width;
	srcRect.w = BARRELSIZE * height;
	srcRect.x = 0;
	srcRect.y = 0;

	barrelMovement(map);

	xpos += speed;
	ypos += gravity;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void Barrel::render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}

void Barrel::newBarrel(int x, int y)
{
	xpos = x * BARRELSIZE;
	ypos = y * BARRELSIZE;
	speed = 0;
	previousSpeed = -BARRELSPEED;
}
