#include "Plane.h"
#include "Game.h"
#include "CollisionComponent.h"


Plane::Plane(class Game* game)
	:Actor (game)
{
	SetPosition(Vector2(64 / 2.0f, 192 + 3 * 64));
	mSprite = new SpriteComponent(this, 200);
	mSprite->SetTexture(mGame->GetTexture("Assets/Airplane.png"));

	mColl = new CollisionComponent(this);
	mColl->SetSize(64, 64);

	mGame->mPlane_vector.push_back(this);

	mMove = new PlaneMove(this);
}


Plane::~Plane()
{
	mGame->mPlane_vector.erase(std::remove(mGame->mPlane_vector.begin(), mGame->mPlane_vector.end(), this), mGame->mPlane_vector.end());
}
