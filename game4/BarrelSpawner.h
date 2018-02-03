#pragma once
#include "Actor.h"
class BarrelSpawner :
	public Actor
{
public:
	BarrelSpawner(Game* game);
	~BarrelSpawner();

	void UpdateActor(float deltaTime) override;

private:
	float timer;
};

