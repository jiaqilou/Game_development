#pragma once
#include "SDL/SDL.h"
#include <string>
// TODO
class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Point paddle;
	SDL_Point ball;
	SDL_Point ballvelocity;

	float deltatime;
	float lasttime, currenttime;
	unsigned int currentt;

	int paddlemovement;
	int uplimit, paddledownlimit, rightlimit,balldownlimit, leftlimit;
public:
	Game();

	bool Initialize();

	void Shutdown();

	void RunLoop();

	bool ProcessInput();

	bool UpdateGame();

	void GenerateOutput();
};