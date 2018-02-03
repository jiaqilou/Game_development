#pragma once
#include "MoveComponent.h"
#include "Math.h"
class PlayerMove :
	public MoveComponent
{
public:
	enum MoveState {
		OnGround,
		Jump,
		Falling,
		WallClimb,
		WallRun
	};

	enum CollSide {
		None,
		Top,
		Bottom,
		SideX1,
		SideX2,
		SideY1,
		SideY2
	};

	PlayerMove(class Actor* owner);
	~PlayerMove() override;

	void Update(float deltaTime) override;

	void ProcessInput(const Uint8* keyState) override;

	void ChangeState(MoveState state);

	void PhysicsUpdate(float deltaTime);

	void AddForce(const Vector3& force);

	void FixXYVelocity();

	bool CanWallClimb(CollSide collside);

	bool CanWallRun(CollSide collside);
private :
	MoveState mCurrentState;
	//float mZSpeed;
	//const float Gravity = -980.f;
	//const float JumpSpeed = 500.0f;
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;
	float mMass;
	Vector3 mGravity;
	Vector3 mJumpForce;
	bool leadingedge ;

	Vector3 mClimbing; 
	float mWallClimbTimer;
	float mWallRunTimer;

	Vector3 mWallRunForce;

	int mRunningSFX;

protected:
	void UpdateOnGround(float deltatime);
	void UpdateJump(float deltatime);
	void UpdateFalling(float deltatime);
	void UpdateWallClimb(float deltatime);
	void UpdateWallRun(float deltatime);
	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);


};

