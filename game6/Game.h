#pragma once
#include "SDL/SDL.h"
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "SpriteComponent.h"
#include "Block.h"
#include "Coin.h"
#include <sstream>
#include <fstream>
#include "Player.h"
#include "Barrel.h"
#include "BarrelSpawner.h"
#include "SDL/SDL_mixer.h"
// TODO
class Actor;

class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Point ballvelocity;

	float deltatime;
	float lasttime, currenttime;
	unsigned int currentt;

	int paddlemovement;
	int uplimit, paddledownlimit, rightlimit,balldownlimit, leftlimit;

	std::vector<Actor*> actor_vector;
	std::unordered_map<std::string, SDL_Texture*> texture_map;
	std::unordered_map<std::string, Mix_Chunk*> sound_map;
	void LoadData();

	void UnloadData();
	std::vector<SpriteComponent*> sprite_vector;


	std::vector<Block*> block_vector;

	Vector2 mCameraPos;
public:
	
	Game();

	bool Initialize();

	void Shutdown();

	void RunLoop();

	bool ProcessInput();

	bool UpdateGame();

	void GenerateOutput();

	void AddActor(Actor* actor);

	void RemoveActor(Actor* actor);

	void LoadTexture(std::string filename);

	SDL_Texture* GetTexture(std::string filename);

	void AddSprite(SpriteComponent* sprite);
	void RemoveSprite(SpriteComponent* sprite);


	void addBlock(Block* block);

	void removeBlock(Block* block);

	std::vector<Block*> get_block_vector();

	Player* mPlayer;

	void LoadSound(const std::string& filename);
	Mix_Chunk* GetSound(const std::string& filename);

	const Vector2& GetCameraPos();
	void SetCameraPos(const Vector2& cameraPos);
};