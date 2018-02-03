//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <iostream>
// TODO
using namespace std;
const int width = 25;
const int paddleheight = 100;
const int paddlespeed = 300;
const int ballV = 200;

Game::Game() {
	lasttime = 0;
	paddlemovement = 0;
	paddle = { 0,320 };
	ball = { 500,370 };
	uplimit = width;
	paddledownlimit = 768 - width - paddleheight;
	balldownlimit = 768 - 2*width;
	rightlimit = 1024 - 2*width;
	ballvelocity = { ballV,ballV };

	leftlimit = width;
}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window =  SDL_CreateWindow("window",100,50,1024,768,0);

	renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return true;
}

void Game::Shutdown(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::RunLoop() {
	bool run = true;
	bool up = true;
	while (run&& up) {
		run = ProcessInput();
		up = UpdateGame();
		GenerateOutput();
	}
}

bool Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		return false;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		paddlemovement++;
	}
	if (state[SDL_SCANCODE_UP]) {
		paddlemovement--;
	}
	return true;
}

bool Game::UpdateGame()
{
	currentt = SDL_GetTicks();
	currenttime = ((float)currentt / (float)1000);
	deltatime = currenttime - lasttime;
	lasttime = currenttime;

	Uint32 timeout = currentt + 16;
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
		//SDL_Log("waitings");
	}

	paddle.y = paddle.y+ paddlespeed * paddlemovement*deltatime;
	if (paddle.y < uplimit) {
		paddle.y = uplimit;
	}
	else if (paddle.y > paddledownlimit) {
		paddle.y = paddledownlimit;
	}
	paddlemovement = 0;
	
	ball.x = ball.x + ballvelocity.x * deltatime;
	ball.y = ball.y + ballvelocity.y * deltatime;
	if (ball.y < uplimit) {
		ball.y = uplimit;
		ballvelocity.y = -ballvelocity.y;
	}
	else if (ball.y > balldownlimit) {
		ball.y = balldownlimit;
		ballvelocity.y = -ballvelocity.y;
	}
	if (ball.x > rightlimit) {
		ball.x = rightlimit;
		ballvelocity.x = -ballvelocity.x;
	}
	else if (ball.x < leftlimit) {
		if (ball.y+width > paddle.y && ball.y < (paddle.y + paddleheight)) {
			ball.x = leftlimit;
			ballvelocity.x = -ballvelocity.x;
		}
	} 
	if (ball.x < 0) {
		return false;
	}
	return true;
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(renderer,0,0,255,255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);
	SDL_Rect toprec;
	toprec.x = 0;
	toprec.y = 0;
	toprec.w = 1024;
	toprec.h = width;
	SDL_RenderFillRect(renderer, &toprec);

	SDL_Rect rightrec;
	rightrec.x = 1024-width;
	rightrec.y = 0;
	rightrec.w = width;
	rightrec.h = 768;
	SDL_RenderFillRect(renderer, &rightrec);

	SDL_Rect botrec;
	botrec.x = 0;
	botrec.y = 768-width;
	botrec.w = 1024;
	botrec.h = width;
	SDL_RenderFillRect(renderer, &botrec);
	
	
	SDL_Rect paddlerec;
	paddlerec.x = paddle.x;
	paddlerec.y = paddle.y;
	paddlerec.w = width;
	paddlerec.h = paddleheight;
	SDL_RenderFillRect(renderer, &paddlerec);

	SDL_Rect ballrec;
	ballrec.x = ball.x;
	ballrec.y = ball.y;
	ballrec.w = width;
	ballrec.h = width;
	SDL_RenderFillRect(renderer, &ballrec);

	SDL_RenderPresent(renderer);
}
