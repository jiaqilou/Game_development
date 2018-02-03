#include "BarrelSpawner.h"
#include "Game.h"


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
		Barrel* barrel = new Barrel(mGame);
		barrel->SetPosition(mPosition);
		mGame->AddSprite(barrel->GetSprite());
		timer = 3.0;
	}
}