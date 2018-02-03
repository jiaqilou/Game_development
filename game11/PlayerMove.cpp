#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include <algorithm>
#include "Checkpoint.h"
PlayerMove::PlayerMove(Actor* owner)
	:MoveComponent(owner)
	//, mZSpeed(0.0)
	, leadingedge(true)
	, mMass(1.0f)
	, mGravity(Vector3(0.0f, 0.0f, -980.0f))
	, mJumpForce(Vector3(0.0f, 0.0f, 35000.0f))
	,mClimbing(Vector3(0.0f, 0.0f, 1800.0f))
	,mWallClimbTimer(0.0f)
	,mWallRunTimer(0.0f)
	, mWallRunForce(Vector3(0.0f,0.0f,1200.0f))
{
	ChangeState(MoveState::Falling);
	mRunningSFX =  Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
	Mix_Pause(mRunningSFX);
}


PlayerMove::~PlayerMove()
{
	printf("child des\n");
	Mix_HaltChannel(mRunningSFX);
}

void PlayerMove::Update(float deltaTime) {
	if ((mVelocity.Length() > 50&& mCurrentState== MoveState::OnGround)|| mCurrentState == MoveState::WallClimb|| mCurrentState == MoveState::WallRun) {
		Mix_Resume(mRunningSFX);
	}
	else {
		Mix_Pause(mRunningSFX);
	}
	if (mCurrentState == MoveState::Falling) {		
		UpdateFalling(deltaTime);
	}
	else if (mCurrentState == MoveState::Jump) {
		UpdateJump(deltaTime);
	}
	else if (mCurrentState == MoveState::OnGround) {
		UpdateOnGround(deltaTime);
	}
	else if (mCurrentState == MoveState::WallClimb) {
		UpdateWallClimb(deltaTime);
	}
	else if (mCurrentState == MoveState::WallRun) {
		UpdateWallRun(deltaTime);
	}
	if (mOwner->GetPosition().z < -750.0f) {
		Player* player = reinterpret_cast<Player*>(mOwner);
		mOwner->SetPosition(player->respawn_position);
		mVelocity = Vector3::Zero;
		mPendingForces = Vector3::Zero;
		ChangeState(MoveState::Falling);
	}
	
	if (mOwner->GetGame()->check_queue.size() > 0) {
		
		if (mOwner->getColl()->Intersect(mOwner->GetGame()->first_check->getColl())) {
			if (!mOwner->GetGame()->first_check->mLevelString.empty()) {
				mOwner->GetGame()->SetString(mOwner->GetGame()->first_check->mLevelString);
			}
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
			Player* player = reinterpret_cast<Player*>(mOwner);
			player->respawn_position = mOwner->GetGame()->first_check->GetPosition();
			mOwner->GetGame()->first_check->active = false;
			mOwner->GetGame()->check_queue.pop();
			mOwner->GetGame()->first_check->SetState(Actor::EDead);
			
			if (mOwner->GetGame()->check_queue.size() > 0) {
				mOwner->GetGame()->first_check = mOwner->GetGame()->check_queue.front();				
				mOwner->GetGame()->first_check->active = true;
			}
		}
	}
	


}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) {
		AddForce(mOwner->GetForward() * 700);
	}
	if (state[SDL_SCANCODE_S]) {
		AddForce(mOwner->GetForward() * -700);
	}
	if (!state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W]) {
		AddForce(mOwner->GetForward() * 0);
	}
	if (state[SDL_SCANCODE_A]) {
		//mStrafeSpeed = -350;
		AddForce(mOwner->GetRight()*-700);
	}
	if (state[SDL_SCANCODE_D]) {
		AddForce(mOwner->GetRight()*700);
	}
	if (!state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A]) {
		AddForce(mOwner->GetRight() * 0);
	}

	if (state[SDL_SCANCODE_SPACE] && leadingedge&& mCurrentState== MoveState::OnGround) {
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
		leadingedge = false;
		AddForce(mJumpForce);
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
	PhysicsUpdate(deltatime);
	bool clide = false;
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::Top) {
			clide = true;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideX1) {
			if (CanWallClimb(PlayerMove::CollSide::SideX1)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideX2) {
			
			if (CanWallClimb(PlayerMove::CollSide::SideX2)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY1) {
			if (CanWallClimb(PlayerMove::CollSide::SideY1)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY2) {
			if (CanWallClimb(PlayerMove::CollSide::SideY2)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
		}
	}
	if (!clide) {
		ChangeState(MoveState::Falling);
	}


}

void PlayerMove::UpdateJump(float deltatime) {
	AddForce(mGravity);
	PhysicsUpdate(deltatime);
	bool collide = false;
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::Bottom) {
			mVelocity.z = 0.0f;	
			collide = true;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::Top) {
			mVelocity.z = 0.0f;
			ChangeState(MoveState::OnGround);
			collide = true;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideX1) {
			if (CanWallRun(PlayerMove::CollSide::SideX1)) {
				mWallRunTimer = 0.0f;
				ChangeState(MoveState::WallRun);
				return;
			}
			if (CanWallClimb(PlayerMove::CollSide::SideX1)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideX2) {

			if (CanWallClimb(PlayerMove::CollSide::SideX2)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
			if (CanWallRun(PlayerMove::CollSide::SideX2)) {
				
				mWallRunTimer = 0.0f;
				ChangeState(MoveState::WallRun);
				return;
			}
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY1) {
			if (CanWallClimb(PlayerMove::CollSide::SideY1)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
			if (CanWallRun(PlayerMove::CollSide::SideY1)) {
				
				mWallRunTimer = 0.0f;
				ChangeState(MoveState::WallRun);
				return;
			}
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY2) {
			if (CanWallClimb(PlayerMove::CollSide::SideY2)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
			if (CanWallRun(PlayerMove::CollSide::SideY2)) {
			
				mWallRunTimer = 0.0f;
				ChangeState(MoveState::WallRun);
				return;
			}
		}
	}
	if (mVelocity.z <= 0.0f && !collide) {
		ChangeState(MoveState::Falling);
	}
}

void PlayerMove::UpdateFalling(float deltatime) {
	AddForce(mGravity);
	PhysicsUpdate(deltatime);
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::Top) {
			mVelocity.z = 0.0f;
			ChangeState(MoveState::OnGround);
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
		}
	}
}

void PlayerMove::UpdateWallClimb(float deltatime) {
	mWallClimbTimer += deltatime;
	if (mWallClimbTimer < 0.4f) {
		AddForce(mClimbing);
	}
	AddForce(mGravity);
	PhysicsUpdate(deltatime);
	bool climbing = false;
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == CollSide::SideX1) {
				climbing = true;
				mVelocity.x *= 1.05;
				break;			
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideX2) {
				mVelocity.x *= 1.1;
				climbing = true;
				break;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY1) {
				mVelocity.y *= 1.2;
				climbing = true;
				break;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY2) {
				mVelocity.y *= 1.3;
				climbing = true;
				break;
		}
	}
	if (!climbing || mVelocity.z <= 0.0f) {
		mVelocity.z = 0.0f;
		mVelocity.x = 0;
		mVelocity.y = 0;
		ChangeState(MoveState::Falling);
	}
}

void PlayerMove::UpdateWallRun(float deltatime) {
	mWallRunTimer += deltatime;
	if (mWallRunTimer < 0.4f) {
		AddForce(mWallRunForce);
	}
	AddForce(mGravity);
	PhysicsUpdate(deltatime);
	for (Block* i : mOwner->GetGame()->block_vector) {
		if (FixCollision(mOwner->getColl(), i->getColl()) == CollSide::SideX1) {
			mVelocity.y *= 1.6;
			break;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideX2) {
			mVelocity.y *= 1.6;
			break;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY1) {
			mVelocity.x *= 1.6;
			break;
		}
		if (FixCollision(mOwner->getColl(), i->getColl()) == PlayerMove::CollSide::SideY2) {
			mVelocity.x *= 1.6;
			break;
		}
	}
	if (mVelocity.z <= 0.0f) {
		mVelocity.z = 0.0f;
		ChangeState(MoveState::Falling);
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
				if (x1 < x2) {
					new_pos.x = mOwner->GetPosition().x - x1;
					mOwner->SetPosition(new_pos);
					AddForce(Vector3(-700, 0, 0));
					return PlayerMove::CollSide::SideX2;
				}
				else {
					new_pos.x = mOwner->GetPosition().x+x2;
					mOwner->SetPosition(new_pos);
					AddForce(Vector3(700, 0, 0));
					return PlayerMove::CollSide::SideX1;
				}				
			}
		}
		else {
			if (std::min(y1, y2) < std::min(x1, x2)) {
				if (y1 < y2) {
					new_pos.y = mOwner->GetPosition().y - y1;
					mOwner->SetPosition(new_pos);
					AddForce(Vector3(0, -700, 0));
					return PlayerMove::CollSide::SideY1;
					
				}
				else {
					new_pos.y = mOwner->GetPosition().y + y2;
					mOwner->SetPosition(new_pos);
					AddForce(Vector3(0, 700, 0));		
					return PlayerMove::CollSide::SideY2;
					
				}
			}
			else {
				if (x1 < x2) {
					new_pos.x = mOwner->GetPosition().x - x1;
					mOwner->SetPosition(new_pos);
					AddForce(Vector3(-700, 0, 0));
					return PlayerMove::CollSide::SideX2;
					
				}
				else {
					new_pos.x = mOwner->GetPosition().x + x2;
					mOwner->SetPosition(new_pos);
					AddForce(Vector3(700, 0, 0));		
					return PlayerMove::CollSide::SideX1;
					
				}
			}
		}
	}
	else {
		return PlayerMove::CollSide::None;
	}
}

void PlayerMove::AddForce(const Vector3& force) {
	mPendingForces = mPendingForces + force;
}

void PlayerMove::PhysicsUpdate(float deltaTime) {
	mAcceleration = mPendingForces*(1.0f / mMass);
	mVelocity += mAcceleration*deltaTime;
	FixXYVelocity();
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity*deltaTime);
	mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed*deltaTime);
	mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity() {
	Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);
	if (xyVelocity.Length() > 400.0f) {
		xyVelocity.Normalize();
		xyVelocity *= 400.0f;
	}
	float factor = 0.9;
	if (mCurrentState == MoveState::OnGround || mCurrentState == MoveState::WallClimb) {
		if (Math::NearZero(mAcceleration.x) || (mAcceleration.x >0 &&xyVelocity.x <0) || 
			(mAcceleration.x <0&& xyVelocity.x>0)) {
			xyVelocity.x *= factor;
		}
		if (Math::NearZero(mAcceleration.y) || (mAcceleration.y >0 && xyVelocity.y <0) ||
			(mAcceleration.y <0 && xyVelocity.y>0)) {
			xyVelocity.y *= factor;
		}
	}
	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}

bool PlayerMove::CanWallClimb(CollSide collside) {
	Vector2 side;
	if (collside == CollSide::SideX1) {
		side = Vector2(-1, 0);
	}
	else if (collside == CollSide::SideX2) {
		side = Vector2(1, 0);
	}
	else if (collside == CollSide::SideY1) {
		side = Vector2(0,1);
	}
	else if (collside == CollSide::SideY2) {
		side = Vector2(0,-1);
	}
	Vector2 forward = Vector2(mOwner->GetForward().x, mOwner->GetForward().y);
	if (Vector2::Dot(side, forward)>0.5 ) {
		Vector2 xy = Vector2(mVelocity.x, mVelocity.y);
		if (xy.Length() > 350.0f && Vector2::Dot(side, xy) > 0) {
			return true;
		}
	}
	return false;
}

bool PlayerMove::CanWallRun(CollSide collside) {
	Vector2 side;
	if (collside == CollSide::SideX1) {
		side = Vector2(-1, 0);
	}
	else if (collside == CollSide::SideX2) {
		side = Vector2(1, 0);
	}
	else if (collside == CollSide::SideY1) {
		side = Vector2(0, 1);
	}
	else if (collside == CollSide::SideY2) {
		side = Vector2(0, -1);
	}
	Vector2 forward = Vector2(mOwner->GetForward().x, mOwner->GetForward().y);
	if ( std::abs ( Vector2::Dot(side, forward)) < 0.3) {
		Vector2 xy = Vector2(mVelocity.x, mVelocity.y);
		if (xy.Length() > 350.0f ) {
			if (Vector2::Dot(xy, forward) > 0.5 ) {
				return true;
			}
		}
	}
	return false;
}