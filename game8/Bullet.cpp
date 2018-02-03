#include "Bullet.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Tank.h"
Bullet::Bullet(Game* game)
	:Actor (game)
{
	mMove = new MoveComponent(this);
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));

	mColl = new CollisionComponent(this);
	mColl->SetSize(10, 10, 10);

	mScale = 0.5;
	mMove->SetForwardSpeed(400);

}


Bullet::~Bullet()
{
}

void Bullet::set_tank(Tank* tank) {
	mTank = tank;
}

void Bullet::UpdateActor(float deltaTime) {
	for (Block* i : mGame->block_vector) {
		if (mColl->Intersect(i->getColl()) ) {
			this->SetState(Actor::EDead);
		}
	}
	
	if (mColl->Intersect(mGame->tank_one->getColl())) {
		if (mTank != mGame->tank_one) {
			mGame->tank_one->Respawn();
			this->SetState(Actor::EDead);
		}
	}

	if (mColl->Intersect(mGame->tank_two->getColl())) {
		if (mTank != mGame->tank_two) {
			mGame->tank_two->Respawn();
			this->SetState(Actor::EDead);
		}
	}
}