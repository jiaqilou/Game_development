//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Block.h"
#include "LevelLoader.h"
#include "Arrow.h"

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
	Renderer* ren = new Renderer(this);
	mRenderer = ren;
	if (!mRenderer->Initialize(1024, 768)) {
		return false;
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	TTF_Init();
	LoadData();

	mTicksCount = SDL_GetTicks();
	SDL_SetRelativeMouseMode(SDL_TRUE); // Enable relative mouse mode
	SDL_GetRelativeMouseState(nullptr, nullptr);

	Arrow* a = new Arrow(this);
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
		if (!mNextLevel.empty())
			LoadNextLevel();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	if (mActors.size() > 0) {
		//printf("error %d \n", mActors.size());
		for (auto actor : copy)
		{
			actor->Update(deltaTime);
		}

		// Add any dead actors to a temp vector
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}
	}
	mHud->Update(deltaTime);

	// Delete any of the dead actors (which will
	// remove them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
	mRenderer->Draw();
}

void Game::LoadData()
{
	//LoadLevel("Assets/Level.txt");
	mHud = new HUD(this);
	LoadSound("Assets/Sounds/Checkpoint.wav");
	LoadSound("Assets/Sounds/Coin.wav");
	LoadSound("Assets/Sounds/Jump.wav");
	LoadSound("Assets/Sounds/Land.wav");
	LoadSound("Assets/Sounds/Music.ogg");
	LoadSound("Assets/Sounds/Running.wav");
	LevelLoader::Load(this,"Assets/Tutorial.json");
	Matrix4 projection_matrix = Matrix4::CreatePerspectiveFOV(1.22,1024,768 , 10, 10000);
	mRenderer->SetProjectionMatrix(projection_matrix);

	Vector3 eye = Vector3(0, 0, 0);
	Vector3 target = Vector3(10, 0, 0);
	Vector3 up = Vector3(0, 0, 10);
	Matrix4 look_at_matrix = Matrix4::CreateLookAt(eye,target,up);
	mRenderer->SetViewMatrix(look_at_matrix);

	first_check = check_queue.front();
	first_check->active = true;
	//my_player->SetPosition(my_player->respawn_position);
	//my_player->respawn_position = first_check->GetPosition();

	Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
}

void Game::LoadLevel(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Failed to load level: %s", fileName.c_str());
	}

	const float topLeftX = -512.0f + 32.0f;
	const float topLeftY = -384.0f + 32.0f;
	size_t row = 0;
	std::string line;
	while (!file.eof())
	{
		std::getline(file, line);
		for (size_t col = 0; col < line.size(); col++)
		{
			// Calculate position at this row/column
			Vector3 pos;
			pos.x = topLeftX + 64.0f * col;
			pos.y = topLeftY + 64.0f * row;
			if (line[col] == 'B')
			{
				// TODO: Create block
				Block* block = new Block(this);
				block->SetPosition(pos);
				
			}
			/*else if (line[col] == 'P')
			{
				// TODO: Create player 1
				tank_one = new Tank(this);
				tank_one->SetPosition(pos);
				tank_one->origin_pos = pos;
			}
			else if (line[col] == 'Q')
			{
				tank_two = new Tank(this);
				tank_two->SetPosition(pos);
				tank_two->SetPlayerTwo();
				tank_two->origin_pos = pos;
				// TODO: Create player 2
			}*/
		}
		row++;
	}
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

void Game::LoadSound(const std::string& fileName)
{
	Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());
	if (!chunk)
	{
		SDL_Log("Failed to load sound file %s", fileName.c_str());
		return;
	}

	mSounds.emplace(fileName, chunk);
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
	mRenderer->Shutdown();
	delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}

void Game::SetString(std::string s) {
	mNextLevel = s;
}

void Game::LoadNextLevel() {
	
	for (auto actor : mActors)
	{
		actor->SetState(Actor::EDead);
	}
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete any of the dead actors (which will
	// remove them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
	mActors.clear();
	block_vector.clear();
	if (check_queue.size() > 0) {
		check_queue.pop();
	}
	//printf("delete all \n");
	LevelLoader::Load(this, mNextLevel);
	//printf("recload \n");
	first_check = check_queue.front();
	first_check->active = true;
	Arrow* a = new Arrow(this);
	mNextLevel.clear();
}
