//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include <iostream>
#include <SDL/SDL_image.h>
// TODO
using namespace std;
const int width = 25;
const int paddleheight = 100;
const int paddlespeed = 300;
const int ballV = 200;

Game::Game() {
	lasttime = 0;
	paddlemovement = 0;
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

	int flags = IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted&flags) != flags) {
		printf("IMG_Init: Failed to init required png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
		return false;
	}
	LoadData();

	return true;
}

void Game::Shutdown(){
	UnloadData();
	IMG_Quit();
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

	/*for (std::vector<Actor*>::iterator it = actor_vector.begin(); it != actor_vector.end(); ++it) {
		(*it) -> Process
	}*/
	for (Actor* i : actor_vector) {
		i->ProcessInput(state);
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

	std::vector<Actor*> actor_vector_copy = actor_vector;
	for (Actor*i : actor_vector_copy) {
		i->Update(deltatime);
	}

	std::vector<Actor*> dead_actor;
	for (Actor*i : actor_vector) {
		if (i->GetState() == Actor::EDead) {
			dead_actor.push_back(i);
		}
	}
	for (Actor*i : dead_actor) {
		delete i;
	}
	return true;
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	SDL_RenderClear(renderer);


	//SDL_RenderCopy(renderer, texture_map["Assets/Stars.png"], NULL, NULL);
	for (SpriteComponent* i : sprite_vector) {
		i->Draw(renderer);
	}

	SDL_RenderPresent(renderer);
}

void Game::AddActor(Actor* actor) {
	if (actor != NULL) {
		actor_vector.push_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	std::vector<Actor*>::iterator it;

	it = std::find(actor_vector.begin(), actor_vector.end(), actor);
	if (it != actor_vector.end()) {
		actor_vector.erase(it);
	}
}

void Game::LoadData() {
	LoadTexture("Assets/Asteroid.png");
	LoadTexture("Assets/Laser.png");
	LoadTexture("Assets/Ship.png");
	LoadTexture("Assets/ShipThrust.png");
	LoadTexture("Assets/Stars.png");

	/*Actor* test = new Actor(this);
	SpriteComponent* sc = new SpriteComponent(test);
	sc->SetTexture(GetTexture("Assets/Ship.png"));
	test->SetSprite(sc);

	Actor* lasertest = new Actor(this);
	lasertest->SetPosition(Vector2(200, 100));
	SpriteComponent* s = new SpriteComponent(lasertest);
	s->SetTexture(GetTexture("Assets/Laser.png"));
	lasertest->SetSprite(s);

	Actor* thrusttest = new Actor(this);
	thrusttest->SetPosition(Vector2(200, 200));
	thrusttest->SetScale(0.75);
	thrusttest->SetRotation(Math::PiOver2);
	SpriteComponent* st = new SpriteComponent(thrusttest);
	st->SetTexture(GetTexture("Assets/ShipThrust.png"));
	thrusttest->SetSprite(st);*/

	Actor* btest = new Actor(this);
	btest->SetPosition(Vector2(512,384));
	SpriteComponent* sb = new SpriteComponent(btest,99);
	sb->SetTexture(GetTexture("Assets/Stars.png"));
	btest->SetSprite(sb);

	Ship* sh = new Ship(this);
	sh->SetPosition(Vector2(512, 300));
	//sh->GetMove()->SetForwardSpeed(50);
	//sh->GetMove()->SetAngularSpeed(0.5);
	AddSprite(sh->GetSprite());


	//AddSprite(sc);
	//AddSprite(s);
	//AddSprite(st);
	AddSprite(sb);
}

void Game::UnloadData() {
	while (!actor_vector.empty()) {
		delete actor_vector.back();
	}

	for (std::pair<std::string, SDL_Texture*> element : texture_map)
	{
		SDL_DestroyTexture(element.second);
	}
	texture_map.clear();
}

void Game::LoadTexture(std::string filename) {
	SDL_Surface *image;
	image = IMG_Load(filename.c_str());
	if (!image) {
		printf("IMG_Load: %s\n", IMG_GetError());
		// handle error
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		//exit(1);
	}
	SDL_FreeSurface(image);
	image = NULL;
	texture_map[filename] = texture;
}

SDL_Texture* Game::GetTexture(std::string filename) {
	std::unordered_map<std::string, SDL_Texture*>::const_iterator it = texture_map.find(filename);

	if (it == texture_map.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	sprite_vector.push_back(sprite);
	std::sort(sprite_vector.begin(), sprite_vector.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	std::vector<SpriteComponent*>::iterator it;
	it = std::find(sprite_vector.begin(), sprite_vector.end(), sprite);
	if (it != sprite_vector.end()) {
		sprite_vector.erase(it);
	}
}