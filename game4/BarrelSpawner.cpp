#include "BarrelSpawner.h"



BarrelSpawner::BarrelSpawner(Game* game)
	:Actor(game)
{
	timer = 3.0;
}


BarrelSpawner::~BarrelSpawner()
{
}

void BarrelSpawner::UpdateActor(float deltaTime) {
	timer = timer - deltaTime;
	if (timer <= 0) {
		timer = 3.0;
	}
}
