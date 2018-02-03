#pragma once
#include "SDL/SDL.h"
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "SpriteComponent.h"
#include <sstream>
#include <fstream>
#include "SDL/SDL_mixer.h"
#include "Math.h"
#include "Grid.h"
#include "Plane.h"
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

	Vector2 mCameraPos;

	
public:
	
	Game();
	Grid* mGrid;
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


	void LoadSound(const std::string& filename);
	Mix_Chunk* GetSound(const std::string& filename);

	const Vector2& GetCameraPos();
	void SetCameraPos(const Vector2& cameraPos);

	std::vector <Plane*> mPlane_vector;
};