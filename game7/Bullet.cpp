#include "Bullet.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

Bullet::Bullet(class Game* game)
	:Actor (game)
	,life(0.0)
{
	mSprite = new SpriteComponent(this, 200);
	mSprite->SetTexture(mGame->GetTexture("Assets/Bullet.png"));

	mColl = new CollisionComponent(this);
	mColl->SetSize(10, 10);

	mMove = new MoveComponent(this);
	mMove->SetForwardSpeed(500);
}


Bullet::~Bullet()
{
}

void Bullet::UpdateActor(float deltaTime) {
	life += deltaTime;
	if (life > 2) {
		SetState(Actor::EDead);
	}
	else {
		for (Plane* p : mGame->mPlane_vector) {
			if (p->getColl()->Intersect(mColl)) {
				p->SetState(Actor::EDead);
				SetState(Actor::EDead);
				break;
			}
		}
	}
}