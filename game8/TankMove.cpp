#include "TankMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include <algorithm>
TankMove::TankMove(class Actor* owner)
	:MoveComponent(owner)
{
	mForwardKey = SDL_SCANCODE_W;
	mBackKey = SDL_SCANCODE_S;
	mLeftKey = SDL_SCANCODE_A;
	mRightKey = SDL_SCANCODE_D;
}


TankMove::~TankMove()
{
}

void TankMove::Update(float deltaTime) {
	MoveComponent::Update(deltaTime);

	for (Block* i : mOwner->GetGame()->block_vector) {
		Vector3 new_pos;
		new_pos.x = mOwner->GetPosition().x;
		new_pos.y = mOwner->GetPosition().y;
		if (mOwner->getColl()->Intersect(i->getColl())) {
			int dx1, dx2, dy1, dy2;
			dy1 = abs(i->GetPosition().y - 32 - (mOwner->GetPosition().y + 15));
			dy2 = abs(mOwner->GetPosition().y - 15 - (i->GetPosition().y + 32));
			dx1 = abs(mOwner->GetPosition().x + 15 - i->GetPosition().x + 32);
			dx2 = abs(i->GetPosition().x + 32 - (mOwner->GetPosition().x - 15));

			
			if (std::min(dy1, dy2) < std::min(dx1, dx2)) {
				if (dy1 < dy2) {
					new_pos.y = new_pos.y - dy1;				
				}
				else {
					new_pos.y = new_pos.y + dy2;					
				}

			}
			else {
				if (dx1 < dx2) {
					new_pos.x = new_pos.x - dx1;
				}
				else {
					new_pos.x = new_pos.x + dx2;
				}
			}
			//printf("position x %f y %f", mOwner->GetPosition().x, mOwner->GetPosition().y);
			mOwner->SetPosition(new_pos);
			//break;
		}
		
	}
}

void TankMove::ProcessInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[mForwardKey]) {
		SetForwardSpeed(250);
	}
	if (state[mBackKey]) {
		SetForwardSpeed(-250);
	}
	if (!state[mForwardKey] && !state[mBackKey]) {
		SetForwardSpeed(0);
	}
	if (state[mLeftKey]) {
		this->SetAngularSpeed(Math::TwoPi);
	}
	if (state[mRightKey]) {
		this->SetAngularSpeed(-Math::TwoPi);
	}
	if (!state[mLeftKey] && !state[mRightKey]) {
		this->SetAngularSpeed(0);
	}
}

void TankMove::SetPlayerTwo() {
	mForwardKey = SDL_SCANCODE_O;
	mBackKey = SDL_SCANCODE_L;
	mLeftKey = SDL_SCANCODE_K;
	mRightKey = SDL_SCANCODE_SEMICOLON;
}