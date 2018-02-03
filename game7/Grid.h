#pragma once
#include "Actor.h"
#include "Tower.h"
#include "Plane.h"
#include <vector>

class Grid : public Actor
{
public:
	Grid(class Game* game);
	// Getters for start/end tiles
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	// Overrides from base
	void ActorInput(const Uint8* keyState) override;
	void UpdateActor(float deltaTime) override;

	bool TryFindPath();

	std::vector <Tile*> openset;


	int calculateDis(Tile* cur, Tile* target);

	void UpdatePathTiles();

	void BuildTower(Tile* tile);
private:
	// Select the tile in the specified row/column
	void SelectTile(size_t row, size_t col);
	// Tile that's currently selected (nullptr if none)
	class Tile* mSelectedTile;
	// 2D grid of actual tiles
	std::vector<std::vector<class Tile*>> mTiles;

	// Constants to adjust grid's properties
	const size_t NumRows = 7;
	const size_t NumCols = 16;
	const float StartY = 192.0f;
	const float TileSize = 64.0f;

	bool mNeedToBuild;

	float timer;
};
