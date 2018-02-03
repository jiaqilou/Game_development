#include "PaddleMove.h"
#include "Actor.h"


PaddleMove::PaddleMove(class Actor* owner)
	:MoveComponent(owner)
{
}


PaddleMove::~PaddleMove()
{
}

void PaddleMove::Update(float deltaTime) {
	mOwner->SetRotation(mOwner->GetRotation() + deltaTime*mAngularSpeed);
	Vector2 new_pos;
	new_pos.x = mOwner->GetForward().x * mForwardSpeed * deltaTime + mOwner->GetPosition().x;
	new_pos.y = mOwner->GetForward().y * mForwardSpeed * deltaTime + mOwner->GetPosition().y;
	
	//Vector2 pos = mOwner->GetPosition();
	if (new_pos.x >= 940) {
		new_pos.x = 940;
	}
	else if (new_pos.x <= 84) {
		new_pos.x = 84;
	}

	mOwner->SetPosition(new_pos);
}