#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "Block.h"
#include <queue>
#include "Checkpoint.h"
#include "Player.h"
#include "Renderer.h"
#include "Coin.h"
#include <SDL/SDL_ttf.h>
#include "HUD.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void LoadSound(const std::string& fileName);
	Mix_Chunk* GetSound(const std::string& fileName);

	void LoadLevel(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }

	std::vector <Block*> block_vector;

	Checkpoint* first_check;
	std::queue <Checkpoint*> check_queue;

	Player* my_player;

	Coin* mCoin;

	void SetString(std::string s);

	void LoadNextLevel();
	std::vector<class Actor*> mActors;
	HUD* mHud;
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;

	std::string mNextLevel;
	std::vector<Actor*> deadActors;

	
};
