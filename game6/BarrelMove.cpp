#include "BarrelMove.h"
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

BarrelMove::BarrelMove(class Actor* owner)
	:MoveComponent(owner)
{
	mForwardSpeed = 100;
	mAngularSpeed = -2 * Math::Pi;
	mYSpeed = 0.0;
	
}


BarrelMove::~BarrelMove()
{
}

void BarrelMove::Update(float deltaTime) {
	float times = 1 / deltaTime;
	Vector2 new_pos;
	new_pos.x =  mForwardSpeed * deltaTime + mOwner->GetPosition().x;
	mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed*deltaTime);
	new_pos.y = mYSpeed*deltaTime + mOwner->GetPosition().y;
	for (Block* i : mOwner->GetGame()->get_block_vector()) {
		if (mOwner->getColl()->Intersect(i->getColl())) {
			mYSpeed = 0;
			int dy1 = abs(i->GetPosition().y - 16 - (mOwner->GetPosition().y + 16));
			new_pos.y = new_pos.y - dy1;
		}
	}

	if (mOwner->getColl()->Intersect(mOwner->GetGame()->mPlayer->getColl())) {
		mOwner->GetGame()->mPlayer->SetPosition(Vector2(32,700));

	}
	mOwner->SetPosition(new_pos);

	if (new_pos.y >= 768) {
		mOwner->SetState(Actor::EDead);
	}
	
	mYSpeed = mYSpeed + 2000 / times;
}
