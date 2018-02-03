#include "BallMove.h"
#include "Actor.h"
#include "Game.h"


BallMove::BallMove(class Actor* owner) 
	:MoveComponent(owner)
{
	mBallVelocity = Vector2(250,-250);
}


BallMove::~BallMove()
{
}

void BallMove::Update(float deltaTime) {
	
	Vector2 new_pos;
	new_pos.x = mBallVelocity.x *  deltaTime + mOwner->GetPosition().x;
	new_pos.y = mBallVelocity.y *  deltaTime + mOwner->GetPosition().y;

	if (new_pos.x >= 982) {
		new_pos.x = 982;
		mBallVelocity.x = -mBallVelocity.x;
	}
	else if (new_pos.x <= 42) {
		new_pos.x = 42;
		mBallVelocity.x = -mBallVelocity.x;
	}
	if (new_pos.y <= 42) {
		new_pos.y = 42;
		mBallVelocity.y = -mBallVelocity.y;
	}
	if (new_pos.y > 790) {
		new_pos = Vector2(512, 717);
		mBallVelocity = Vector2(250, -250);
	}
	if (mOwner->getColl()->Intersect(mOwner->GetGame()->getPaddle()->getColl()) && mBallVelocity.y > 0) {
		Vector2 normal(0,-1);
		if (new_pos.x <= (mOwner->GetGame()->getPaddle()->GetPosition().x- mOwner->GetGame()->getPaddle()->getColl()->GetWidth()/3)) {
			if (mBallVelocity.x < 0) {
				normal = Vector2(-mBallVelocity.y / 4, mBallVelocity.x);
			}
			else if (mBallVelocity.x > 0) {
				normal = Vector2(-mBallVelocity.y / 4, -mBallVelocity.x);
			}	
			//normal = Vector2(1, -1);
			normal.Normalize();
		}
		else if (new_pos.x >( mOwner->GetGame()->getPaddle()->GetPosition().x + mOwner->GetGame()->getPaddle()->getColl()->GetWidth() / 3)) {
			if (mBallVelocity.x > 0) {
				normal = Vector2(mBallVelocity.y / 4, -mBallVelocity.x);
			}
			else if (mBallVelocity.x < 0) {
				normal = Vector2(mBallVelocity.y / 4, mBallVelocity.x);
			}		
			//normal = Vector2(-1, -1);
			normal.Normalize();
		}
		mBallVelocity = Vector2::Reflect(mBallVelocity, normal);
		new_pos = Vector2(new_pos.x, 717);
	}

	for (Block* i : mOwner->GetGame()->get_block_vector()) {
		if (mOwner->getColl()->Intersect(i->getColl())) {
			mOwner->GetGame()->removeBlock(i);
			Vector2 direction(new_pos.x - i->GetPosition().x, new_pos.y - i->GetPosition().y);
			float angle = Math::Atan2(-direction.y, direction.x);
			
			float angle_one = Math::Atan2(1, 2);
			float angle_two = Math::Atan2(1, -2);
			float angle_three = Math::Atan2(-1, -2);
			float angle_four = Math::Atan2(-1, 2);

			if ((angle_four < angle&& angle < angle_one) || (angle_two< angle&&angle <= Math::Pi)||(-Math::Pi<=angle && angle<angle_three)) {
				mBallVelocity.x = -mBallVelocity.x;
			}
			else {
				mBallVelocity.y = -mBallVelocity.y;
			}
			break;
		}
	}
	mOwner->SetPosition(new_pos);
}