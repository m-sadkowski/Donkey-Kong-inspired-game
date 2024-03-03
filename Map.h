#pragma once

#include<SDL.h>

#include "GameObject.h"
#include "Defines.h"

class Map
{
public:
	Map();
	~Map();

	void LoadMap(SDL_Renderer* renderer);
	void renderMap();
	void updateMap();

	const GameObject& getPlatform(int index) const;
	const GameObject& getLadder(int index) const;

private:
	GameObject* platform;
	GameObject* ladder;
	SDL_Renderer* renderer;
};
