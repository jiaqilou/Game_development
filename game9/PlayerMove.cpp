#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include <algorithm>
PlayerMove::PlayerMove(Actor* owner)
	:MoveComponent(owner)
	, mZSpeed(0.0)
	, leadingedge(true)
{
	ChangeState(MoveState::Falling);
}


PlayerMove::~PlayerMove()
{
}

void PlayerMove::Update(float deltaTime) {
	if (mCurrentState == MoveState::Falling) {
		UpdateFalling(deltaTime);
	}
	else if (mCurrentState == MoveState::Jump) {
		UpdateJump(deltaTime);
	}
	else if (mCurrentState == MoveState::OnGround) {
		UpdateOnGround(deltaTime);
	}
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) {
		SetForwardSpeed(350);
	}
	if (state[SDL_SCANCODE_S]) {
		SetForwardSpeed(-350);
	}
	if (!state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W]) {
		SetForwardSpeed(0);
	}
	if (state[SDL_SCANCODE_A]) {
		mStrafeSpeed = -350;
	}
	if (state[SDL_SCANCODE_D]) {
		mStrafeSpeed = 350;
	}
	if (!state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A]) {
		mStrafeSpeed = 0;
	}

	if (state[SDL_SCANCODE_SPACE] && leadingedge&& mCurrentState== MoveState::OnGround) {
		leadingedge = false;
		mZSpeed = JumpSpeed;
		ChangeState(MoveState::Jump);
	}
	else if (!state[SDL_SCANCODE_SPACE]) {
		leadingedge = true;
	}



	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	float angular = (float)x / 500.0f * Math::Pi*10.0f;
	SetAngularSpeed(angular);

	float speed = (float)y / 500.0f * Math::Pi*10.0f;
	mOwner->GetCamera()->SetPitchSpeed(speed);
}

void PlayerMove::ChangeState(MoveState state) {
	mCurrentState = state;
}

void PlayerMove::UpdateOnGround(float deltatime) {
	MoveComponent::Update(deltatime);
	mOwner->SetPosition(Vector3(mOwner->GetPosition().x, mOwner->GetPosition().y, mOwner->GetPosition().z + mZSpeed*deltatime));
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::None) {		
			ChangeState(MoveState::Falling);
		}
	}
}

void PlayerMove::UpdateJump(float deltatime) {
	MoveComponent::Update(deltatime);
	mZSpeed += Gravity*deltatime;
	mOwner->SetPosition(Vector3(mOwner->GetPosition().x, mOwner->GetPosition().y, mOwner->GetPosition().z + mZSpeed*deltatime));
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::Bottom) {
			mZSpeed = 0.0f;			
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::Top) {
			mZSpeed = 0.0f;
			ChangeState(MoveState::OnGround);
		}
	}
}

void PlayerMove::UpdateFalling(float deltatime) {
	MoveComponent::Update(deltatime);
	mZSpeed += Gravity*deltatime;
	mOwner->SetPosition(Vector3(mOwner->GetPosition().x, mOwner->GetPosition().y, mOwner->GetPosition().z + mZSpeed*deltatime));
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::Top) {
			mZSpeed = 0.0f;
			ChangeState(MoveState::OnGround);
		}
	}
}

PlayerMove::CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block) {
	if (self->Intersect(block)) {
		Vector3 new_pos;
		new_pos.x = mOwner->GetPosition().x;
		new_pos.y = mOwner->GetPosition().y;
		new_pos.z = mOwner->GetPosition().z;

		Vector3 blockmin = block ->GetMin();
		Vector3 blockmax = block->GetMax();
		Vector3 selfmin = self->GetMin();
		Vector3 selfmax = self->GetMax();

		float y1 = abs(selfmax.y - blockmin.y);
		float y2 = abs(selfmin.y - blockmax.y);

		float z1 = abs(selfmin.z - blockmax.z);
		float z2 = abs(selfmax.z - blockmin.z);

		float x1 = abs(selfmax.x - blockmin.x);
		float x2 = abs(selfmin.x - blockmax.x);

		if (std::min(z1, z2) < std::min(y1, y2)) {
			
			if (std::min(z1, z2) < std::min(x1, x2)) {
				//printf("z %f", std::min(z1, z2));
				if (z1 < z2) {
					new_pos.z = mOwner->GetPosition().z + z1;
					mOwner->SetPosition(new_pos);
					return PlayerMove::CollSide::Top;
				}
				else {
					new_pos.z = mOwner->GetPosition().z - z2;
					mOwner->SetPosition(new_pos);
					return PlayerMove::CollSide::Bottom;
				}
			}
			else {
				//printf("x %f", std::min(x1, x2));
				if (x1 < x2) {
					new_pos.x = mOwner->GetPosition().x - x1;
				}
				else {
					new_pos.x = mOwner->GetPosition().x+x2;
				}
				mOwner->SetPosition(new_pos);
				return PlayerMove::CollSide::Side;
			}
		}
		else {
			//printf("y %f", std::min(y1, y2));
			if (std::min(y1, y2) < std::min(x1, x2)) {
				if (y1 < y2) {
					new_pos.y = mOwner->GetPosition().y - y1;
				}
				else {
					new_pos.y = mOwner->GetPosition().y + y2;
				}
			}
			else {
				//printf("x %f", std::min(x1, x2));
				if (x1 < x2) {
					new_pos.x = mOwner->GetPosition().x - x1;
				}
				else {
					new_pos.x = mOwner->GetPosition().x + x2;
				}
			}
			mOwner->SetPosition(new_pos);
			return PlayerMove::CollSide::Side;
		}
	}
	else {
		return PlayerMove::CollSide::None;
	}
}