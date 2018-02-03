#include "Barrel.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

Barrel::Barrel(Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this, 150);
	mSprite->SetTexture(mGame->GetTexture("Assets/Barrel.png"));
	mColl = new CollisionComponent(this);
	mColl->SetSize(32, 32);

	mMove = new BarrelMove(this);
}


Barrel::~Barrel()
{
}
