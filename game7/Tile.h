
#pragma once
#include "Actor.h"
#include <vector>

class Tile : public Actor
{
public:
	friend class Grid;
	enum TileState
	{
		EDefault,
		EPath,
		EStart,
		EBase
	};
	
	Tile(class Game* game);
	
	void SetTileState(TileState state);
	TileState GetTileState() const { return mTileState; }
	void ToggleSelect();
	const Tile* GetParent() const { return mParent; }

	bool mInClosedSet;

	float f;
	float g;
	float h;
	Tile* mParent;
private:
	// For pathfinding
	// Adjacency vector
	std::vector<Tile*> mAdjacent;
	// Pointer to parent Tile on path
	
	// For saving f(x), g(x), h(x) values
	
	// Set to true if in closed set
	
	// Set to true if this tile is blocked (impassible)
	bool mBlocked;
	
	// Updates the tile to the correct texture
	void UpdateTexture();
	// Set the state of the tile
	TileState mTileState;
	// Whether or not the user clicked on this tile
	bool mSelected;
};
