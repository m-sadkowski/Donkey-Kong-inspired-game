#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, double x, double y, int timesh, int timesw, float ANGLE)
{
	renderer = ren;
	objTexture = TextureManager::LoadTexture(texturesheet, ren);

	xpos = x * (SIZE / 2);
	ypos = y * (SIZE / 2);
	numberh = timesh;
	numberw = timesw;
	angle = ANGLE;
}

GameObject::~GameObject() {}

void GameObject::update()
{
	srcRect.h = (SIZE / 2) * numberh;
	srcRect.w = (SIZE / 2) * numberw;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = int(xpos);
	destRect.y = int(ypos);
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void GameObject::render()
{
	SDL_RenderCopyEx(renderer, objTexture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}

void GameObject::changeTex(const char* texturesheet)
{
	SDL_DestroyTexture(objTexture);
	objTexture = TextureManager::LoadTexture(texturesheet, renderer);
}
