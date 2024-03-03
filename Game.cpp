#include "Game.h"
#include "GameObject.h"
#include "Player.h"
#include "Map.h"
#include "Barrel.h"
#include<string.h>

const int BARRELS = 12; // NUMBER OF BARRELS IN THE GAME

GameObject* princess;
GameObject* king;
GameObject* barrel;
Barrel* barrels[BARRELS];
Player* player;

Map map;

int min(int x, int y)
{
	if (x < y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	}
}

Game::Game() {}

Game::~Game()
 {
 	clean();
 }

void Game::restartGame()
{
	delete player;
	delete king;
	delete princess;
	delete barrel;
	player = nullptr;
    king = nullptr;
    princess = nullptr;
    barrel = nullptr;
	for (int i = 0; i < numOfBarrels; i++)
	{
		delete barrels[i];
	}
	SDL_RenderClear(renderer);
	SDL_FillRect(screen, NULL, 0);
	numOfBarrels = 0;
	barrelSpawnAcceleration = 0;
	cnt = 0;
	endingFrame = 0;
	direction = 0;
	slow = 0;
	player = new Player("assets/playerRight1.bmp", renderer, 0, 16, CHARACTER, CHARACTER);
	createGameObjects(renderer);
	map.LoadMap(renderer);
	isRunning = true;
}

void Game::createGameObjects(SDL_Renderer* renderer)
{
	princess = new GameObject("assets/princess.bmp", renderer, 36, 0, 5, 5, 0);
	king = new GameObject("assets/king1.bmp", renderer, 36, 4, 7, 8, 0);
	barrel = new GameObject("assets/barrels.bmp", renderer, 44, 7, 4, 4, 0);

	menu = SDL_LoadBMP("assets/cs8x8.bmp");
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) 
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		printf("Subsystems initialized!\n");

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) 
		{
			printf("Window created!\n");
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) 
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			printf("Renderer created!\n");
		}
		isRunning = true;

		screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		player = new Player("assets/playerRight1.bmp", renderer, 0, 16, CHARACTER, CHARACTER);
		createGameObjects(renderer);
		map.LoadMap(renderer);
	}
	else 
	{
		isRunning = false;
	}
}

void Game::winOrLose(SDL_Rect* playerRect)
{
	int end = 0;
	SDL_Rect princessRect = princess->getRect();
	for (int i = 0; i < numOfBarrels; i++)
	{
		SDL_Rect barrelRect = barrels[i]->getRect();
		if (SDL_HasIntersection(playerRect, &barrelRect))
		{
			end = 1;
		}
	}
	if (SDL_HasIntersection(playerRect, &princessRect))
	{
		end = 2;
	}
	if (end) {
		endingFrame = cnt;
		if (end == 1)
		{
			player->end(0);
			printf("Collision with barrel\n");
			player->changeTex("assets/dead.bmp");
			update();
			render();
			SDL_Delay(3000);
			restartGame();
		}
		if (end == 2)
		{
			player->end(1);
			printf("Collision with princess\n");
			player->changeTex("assets/win.bmp");
			update();
			render();
			SDL_Delay(3000);
			isRunning = 0;
		}
	}
}

void Game::spawnBarrels()
{
	if (cnt % (BARREL_SPAWN - barrelSpawnAcceleration) == 0)
	{
		if (numOfBarrels < BARRELS)
		{
			barrels[numOfBarrels] = new Barrel("assets/barrel.bmp", renderer, 20, 1, 1, 1);
			numOfBarrels++;
		}
		else
		{
			for (int i = 0; i < numOfBarrels; i++)
			{
				if (barrels[i]->xpos <= 0)
				{
					barrels[i]->newBarrel(20, 1);
				}
			}
		}
		king->changeTex("assets/king1.bmp");
	}
	if (cnt % (BARREL_SPAWN - barrelSpawnAcceleration) == 20)
	{
		king->changeTex("assets/king2.bmp");
	}
}

void Game::stopPlayer()
{
	if (slow) {
		if (player->checkCollisionWithLadders(map))
		{
			player->speed = 0;
			slow = 0;
		}
		if ((cnt == endingFrame - 5) && (cnt != endingFrame))
		{
			player->speed += direction;
		}
		if ((cnt == endingFrame - 10) && (cnt != endingFrame))
		{
			player->speed += direction;
		}
		if ((cnt == endingFrame - 15) && (cnt != endingFrame))
		{
			player->speed += direction;
		}
		if (cnt >= endingFrame)
		{
			player->speed = 0;
			slow = 0;
		}
	}
}

void Game::blockMovement()
{
	if (isWSKeyPressed && player->checkBottomCollision(map) != 2)
	{
		player->climb = 0;
	}

	if (isADKeyPressed && (player->getX() < 0 || player->getX() > SCREEN_WIDTH - 32 * CHARACTER - 1))
	{
		slow = false;
		player->speed = 0;
	}

	if (!player->checkCollisionWithLadders(map) && player->checkBottomCollision(map) != 2)
	{
		player->climb = 0;
	}

	if (player->getX() < 0)
	{
		player->goBack(0);
	}

	if (player->getX() > SCREEN_WIDTH - 32 * CHARACTER - 1)
	{
		player->goBack(int(SCREEN_WIDTH - 32 * CHARACTER - 1));
	}
}

void Game::animateBarrels()
{
	for (int i = 0; i < numOfBarrels; i++)
	{
		if (!barrels[i]->falling && cnt % 8 == 4)
		{
			barrels[i]->changeTex("assets/barrel1.bmp");
		}
		else if (cnt % 8 == 0)
		{
			barrels[i]->changeTex("assets/barrel.bmp");
		}
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	SDL_Rect playerRect = player->getRect();

	winOrLose(&playerRect);

	spawnBarrels();

	stopPlayer();

	if (event.type == SDL_QUIT) {
		isRunning = 0;
	}
	else if (event.type == SDL_KEYDOWN) {
		// Obs³uga naciœniêcia klawisza
		slow = 0;
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			isADKeyPressed = true;
			if (player->getX() > 0)
			{
				player->goLeft(4 * PLAYERSPEED);
			}
			break;
		case SDLK_RIGHT:
			isADKeyPressed = true;
			if (player->getX() < SCREEN_WIDTH)
			{
				player->goRight(4 * PLAYERSPEED);
			}
			break;
		case SDLK_UP:
			if (player->checkCollisionWithLadders(map) && !player->isJumping) // USUNIÊCIE !player->isJumping POWODUJE MOZLIWOSC ZATRZYMANIA NA DRABINIE
			{
				player->goUp(4 * PLAYERSPEED);
			}
			break;
		case SDLK_DOWN:
			if (player->checkBottomCollision(map) == 2 && !player->isJumping) // mechanizm WS key // USUNIÊCIE !player->isJumping POWODUJE MOZLIWOSC ZATRZYMANIA NA DRABINIE
			{
				player->goDown(4 * PLAYERSPEED);
			}
			isWSKeyPressed = true;
			break;
		case SDLK_SPACE:
			player->jump(map);
			break;
		case SDLK_ESCAPE:
			isRunning = 0;
			break;
		case SDLK_n:
			nextGame = true;
			break;
		default:
			break;
		}
	}
	else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			isADKeyPressed = false;
			endingFrame = cnt + TIME;
			direction = 1;
			slow = true;
			break;
		case SDLK_RIGHT:
			isADKeyPressed = false;
			endingFrame = cnt + TIME;
			direction = -1;
			slow = true;
			break;
		case SDLK_UP:
			player->climb = 0;
			break;
		case SDLK_DOWN:
			player->climb = 0;
			isWSKeyPressed = false;
			break;
		case SDLK_SPACE:
			break;
		default:
			break;
		}
	}

	blockMovement();

	animateBarrels();
}

void Game::update() 
{
	map.updateMap();
	player->update(map, cnt);
	princess->update();
	barrel->update();
	king->update();
	for (int i = 0; i < numOfBarrels; i++)
	{
		if (barrels[i]->xpos > 0)
		{
			barrels[i]->update(map);
		}
	}
	cnt++;
	time = cnt / 60.0;
	//printf("Time: %.2f\n", time);
}

void Game::render()
{
	SDL_RenderClear(renderer);
	map.renderMap();
	princess->render();
	barrel->render();
	king->render();
	for (int i = 0; i < numOfBarrels; i++)
	{
		if (barrels[i]->xpos > 0)
		{
			barrels[i]->render();
		}
	}
	player->render();

	SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, screen);
	SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
	SDL_DestroyTexture(screenTexture);
	DrawString(screen, MENU, 1 * MENU, "Michal Sadkowski 197776", menu);
	DrawString(screen, MENU, 2 * MENU, "n - rozpocznij gre od nowa", menu);
	DrawString(screen, MENU, 3 * MENU, "esc - wyjdz z gry", menu);
	char timeText[128];
	sprintf_s(timeText, "czas: %.2lf s", time);
	DrawString(screen, MENU, 4 * MENU, timeText, menu);
	DrawString(screen, MENU, 5 * MENU, "wykonane podpunkty: A, C, E", menu);
	SDL_RenderPresent(renderer);
}

void Game::clean() 
{
	delete player;
	delete king;
	delete princess;
	delete barrel;
	SDL_FreeSurface(menu);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	printf("Game cleaned!\n");
}
