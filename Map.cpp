#include "Map.h"
#include<stdio.h>

Map::Map() 
{
	platform = new GameObject[PLATFORMS];
	ladder = new GameObject[LADDERS];
}

Map::~Map() {}

void Map::LoadMap(SDL_Renderer* renderer) //50 x 40
{
	platform[0] = GameObject("assets/platform.bmp", renderer, 30, 5, 1, 12, 0);
	platform[1] = GameObject("assets/platform.bmp", renderer, 6, 11, 1, 42, 0);
	platform[2] = GameObject("assets/platform.bmp", renderer, 2, 18, 1, 42, 0);
	platform[3] = GameObject("assets/platform.bmp", renderer, 6, 25, 1, 42, 0); // -5
	platform[4] = GameObject("assets/platform.bmp", renderer, 2, 32, 1, 42, 0); // 5
	platform[5] = GameObject("assets/platform.bmp", renderer, 0, 39, 1, 50, 0);

	ladder[0] = GameObject("assets/ladder.bmp", renderer, 9, 10.9, 7, 3, 0);
	ladder[1] = GameObject("assets/ladder.bmp", renderer, 38, 17.9, 7, 3, 0);
	ladder[2] = GameObject("assets/ladder.bmp", renderer, 9, 24.9, 7, 3, 0);
	ladder[3] = GameObject("assets/ladder.bmp", renderer, 38, 31.9, 7, 3, 0);
	ladder[4] = GameObject("assets/ladder.bmp", renderer, 32, 5, 6, 3, 0);
}

void Map::renderMap()
{
	for (int i = 0; i < PLATFORMS; i++)
	{
		platform[i].render();
	}
	for (int i = 0; i < LADDERS; i++)
	{
		ladder[i].render();
	}
}

void Map::updateMap()
{
	for (int i = 0; i < PLATFORMS; i++)
	{
		platform[i].update();
	}
	for (int i = 0; i < LADDERS; i++)
	{
		ladder[i].update();
	}
}

const GameObject& Map::getPlatform(int index) const
{
	return platform[index];
}

const GameObject& Map::getLadder(int index) const
{
	return ladder[index];
}