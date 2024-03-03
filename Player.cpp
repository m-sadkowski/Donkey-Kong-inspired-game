#include "Player.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include <stdio.h>

const int jumpHeight = 2 * SIZE;

Player::Player(const char* texturesheet, SDL_Renderer* ren, int x, int y, double timesh, double timesw)
{
	renderer = ren;
	objTexture = TextureManager::LoadTexture(texturesheet, ren);

	xpos = x * SIZE;
	ypos = y * SIZE;
	width = timesh;
	height = timesw;
	speed = 0;
	climb = 0;
}

Player::~Player() {}

bool Player::checkCollisionWithPlatforms(const Map& map)
{
	for (int i = 0; i < PLATFORMS; i++)
	{
		const SDL_Rect& platformRect = map.getPlatform(i).getDestRect();

		if (SDL_HasIntersection(&destRect, &platformRect))
		{
			//printf("Collision with platform %d\n", i);
			return true;
		}
	}
	return false;
}

bool Player::checkCollisionWithLadders(const Map& map)
{
	for (int i = 0; i < LADDERS; i++)
	{
		if (SDL_HasIntersection(&destRect, &map.getLadder(i).getDestRect()))
		{
			//printf("Collision with ladder %d\n", i);
			return true;
		}
	}
	return false;
}

int Player::checkBottomCollision(const Map& map)
{

	for (int i = 0; i < LADDERS; i++)
	{
		const SDL_Rect& ladderRect = map.getLadder(i).getDestRect();

		if ((destRect.y + destRect.h <= ladderRect.y + ladderRect.h) && (destRect.y + destRect.h >= ladderRect.y))
		{
			if ((destRect.x + destRect.w - SIZE / 2 >= ladderRect.x && destRect.x + SIZE / 2 <= ladderRect.x + ladderRect.w))
			{
				//printf("Botom collides with ladder %d\n", i);
				return 2;
			}
		}
	}

	for (int i = 0; i < PLATFORMS; i++)
	{
		const SDL_Rect& platformRect = map.getPlatform(i).getDestRect();

		if (destRect.y + destRect.h == platformRect.y)
		{
			if ((destRect.x + destRect.w - SIZE / 2 > platformRect.x && destRect.x + SIZE / 2 < platformRect.x + platformRect.w))
			{
				//printf("Botom collides with platform %d\n", i);
				return 1;
			}
		}
	}

	return 0;
}

void Player::changeTex(const char* texturesheet)
{
	SDL_DestroyTexture(objTexture);
	objTexture = TextureManager::LoadTexture(texturesheet, renderer);
}

void Player::end(bool win)
{
	speed = 0;
	climb = 0;
	gravity = 0;
	isJumping = 0;
	falling = 0;
	xpos = SCREEN_WIDTH / 2 - SIZE * GROW;
	ypos = SCREEN_HEIGHT / 2 - SIZE * GROW;
	width *= GROW;
	height *= GROW;
	if (win)
	{
		height *= PLAYER_GROW;
	}
}

void Player::goBack(int x)
{
	xpos = x;
}

void Player::jump(const Map& map)
{
	if (!isJumping /*&& !checkCollisionWithLadders(map)*/ && !falling && checkBottomCollision(map) != 0)
	{
		isJumping = true;
		jumpStart = ypos;
		falling = false;
		gravity = -GRAVITY;
	}
}

void Player::goLeft(int velocity)
{
	speed = -velocity;
}

void Player::goRight(int velocity)
{
	speed = velocity;
}

void Player::goDown(int velocity)
{
	climb = velocity;
}

void Player::goUp(int velocity)
{
	climb = -velocity;
}

void Player::playerMovement(const Map& map)
{
	if (isJumping)
	{
		/*if (checkBottomCollision(map) == 2) // IF PLAYER JUMPS ON LADDER, HE STOPS FALLING
		{
			isJumping = false;
			falling = false; // IF NOT COMMENTED, PLAYER CAN LAND ON LADDER
			gravity = 0;
		}*/
		if ((ypos == jumpStart /*|| checkBottomCollision(map) == 2*/) && falling == true)  // IF PLAYER JUMPS AND FALLS ON LADDER, HE STOPS FALLING
		{
			isJumping = false;
			falling = false;
			gravity = 0;
		}
		if (ypos <= jumpStart - jumpHeight)  // IF PLAYER REACH THE HEIGHT OF JUMP, HE STARTS TO FALL
		{
			gravity = GRAVITY;
			falling = true;
		}
	}
	else
	{
		if (checkBottomCollision(map) != 0 || checkCollisionWithLadders(map))
		{
			gravity = 0;
		}
		else
		{
			gravity = GRAVITY;
		}
	}
}

void Player::update(const Map& map, int cnt)
{
	srcRect.h = SIZE * width;	
	srcRect.w = SIZE * height;
	srcRect.x = 0;
	srcRect.y = 0;

	playerMovement(map);


	if (isJumping || falling || gravity != 0)
	{
		changeTex("assets/jumping.bmp");
	}
	else if (climb < 0 || climb > 0)
	{
		changeTex("assets/playerUp1.bmp");
		if (cnt % 3 == 1)
		{
			changeTex("assets/playerUp2.bmp");
		}
	}
	else if (speed > 0)
	{
		changeTex("assets/playerRight1.bmp");
		if (cnt % 8 >= 4)
		{
			changeTex("assets/playerRight2.bmp");
		}
	}
	else if (speed < 0)
	{
		changeTex("assets/playerLeft1.bmp");
		if (cnt % 8 >= 4)
		{
			changeTex("assets/playerLeft2.bmp");
		}
	}

	xpos += speed;
	ypos += climb;
	ypos += gravity;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void Player::render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
