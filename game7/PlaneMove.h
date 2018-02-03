#pragma once
#include "MoveComponent.h"
#include "Tile.h"
class PlaneMove :
	public MoveComponent
{
public:
	PlaneMove(class Actor* owner);
	~PlaneMove();

	void Update(float deltaTime) override;

	void SetNextTile(const Tile* tile);
private:
	const Tile* mNextTile;
};

