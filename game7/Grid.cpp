#include "Grid.h"
#include "Tile.h"
#include <SDL/SDL.h>
#include <algorithm>

Grid::Grid(class Game* game)
	:Actor(game)
	,mSelectedTile(nullptr)
	, mNeedToBuild (false)
	,timer (0.0)
{
	// 7 rows, 16 columns
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}
	
	// Create tiles
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(Vector2(TileSize/2.0f + j * TileSize, StartY + i * TileSize));
		}
	}
	
	// Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);
	
	// Set up adjacency lists
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i-1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i+1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j-1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j+1]);
			}
		}
	}
	UpdatePathTiles();
}

void Grid::SelectTile(size_t row, size_t col)
{
	// Make sure it's a valid selection
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		// Deselect previous one
		if (mSelectedTile)
		{
			mSelectedTile->ToggleSelect();
		}
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

Tile* Grid::GetStartTile()
{
	return mTiles[3][0];
}

Tile* Grid::GetEndTile()
{
	return mTiles[3][15];
}

void Grid::ActorInput(const Uint8 * keyState)
{
	// Process mouse click to select a tile
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
	{
		// Calculate the x/y indices into the grid
		y -= static_cast<int>(StartY - TileSize / 2);
		if (y >= 0)
		{
			x /= static_cast<int>(TileSize);
			y /= static_cast<int>(TileSize);
			if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
			{
				SelectTile(y, x);
			}
		}
	}
	bool leadingedge = true;
	if (state[SDL_SCANCODE_SPACE] && leadingedge) {
		leadingedge = false;
		if (mSelectedTile != NULL) {
			mNeedToBuild = true;
		}
	}
	else if (!state[SDL_SCANCODE_SPACE]) {
		leadingedge = true;
	}

}

void Grid::UpdateActor(float deltaTime)
{
	if (mNeedToBuild) {
		BuildTower(mSelectedTile);
		mNeedToBuild = false;
	}

	timer += deltaTime;
	if (timer >= 1) {
		timer = 0;
		Plane* plane = new Plane(mGame);
	}
}

bool Grid::TryFindPath() {
	for (int i = 0; i < mTiles.size(); i++) {
		for (Tile* t : mTiles[i]) {
			t->mInClosedSet = false;
			t->g = 0.0;
		}
	}
	openset.clear();

	Tile* currentTile = GetEndTile();
	currentTile->mInClosedSet = true;

	while (currentTile != GetStartTile()) {
		for (Tile* i : currentTile->mAdjacent) {
			if (i->mInClosedSet || i->mBlocked) {
				continue;
			}
			else {
				bool found = false;
				for (Tile* tile : openset) {
					if (tile == i) {
						found = true;
						float new_g = currentTile->g + TileSize;
						if (new_g < i->g) {
							i->mParent = currentTile;
							i->g = new_g;
							i->f = i->g + i->h;
						}
						break;
					}					
				}
				if (!found) {
					i->mParent = currentTile;
					i->h = calculateDis(i, GetStartTile());
					i->g = currentTile->g + TileSize;
					i->f = i->g + i->h;
					openset.push_back(i);
				}
			}
		}

		if (openset.empty()) {
			return false;
		}

		currentTile = openset[0];
		int index = 0;
		for (int i = 0;i <openset.size(); i++) {
			if (openset[i]->f < currentTile->f) {
				currentTile = openset[i];
				index = i;
			}
		}
		openset.erase(openset.begin() + index);
		currentTile->mInClosedSet = true;
	}
	
	return true;
}

int Grid::calculateDis(Tile* cur, Tile* target) {
	int x_dis = abs (cur->GetPosition().x - target->GetPosition().x);
	int y_dis = abs( cur->GetPosition().y - target->GetPosition().y);

	return x_dis + y_dis;
}

void Grid::UpdatePathTiles() {
	for (int i = 0; i < mTiles.size(); i++) {
		for (Tile* t : mTiles[i]) {
			if (t != GetEndTile() && t != GetStartTile()) {
				t->SetTileState(Tile::EDefault);
			}
		}
	}
	if (TryFindPath()) {
		Tile* current = GetStartTile();
		while (current != GetEndTile()) {
			if (current != GetStartTile() && current != GetEndTile()) {
				current->SetTileState(Tile::EPath);				
			}
			current = current->mParent;
		}
	}
}

void Grid::BuildTower(Tile* tile) {
	if (tile->mBlocked) {
		return;
	}
	else {
		tile->mBlocked = true;
		if (TryFindPath()) {
			UpdatePathTiles();
			Tower* tower = new Tower(mGame);
			tower->SetPosition(mSelectedTile->GetPosition());
			
		}
		else {
			tile->mBlocked = false;
			TryFindPath();
		}
		
	}
}