#pragma once
#include "MoveComponent.h"
class PlayerMove :
	public MoveComponent
{
public:
	enum MoveState {
		OnGround,
		Jump,
		Falling
	};

	enum CollSide {
		None,
		Top,
		Bottom,
		Side
	};

	PlayerMove(class Actor* owner);
	~PlayerMove();

	void Update(float deltaTime) override;

	void ProcessInput(const Uint8* keyState) override;

	void ChangeState(MoveState state);

private :
	MoveState mCurrentState;
	float mZSpeed;
	const float Gravity = -980.f;
	const float JumpSpeed = 500.0f;
	bool leadingedge ;
protected:
	void UpdateOnGround(float deltatime);
	void UpdateJump(float deltatime);
	void UpdateFalling(float deltatime);
	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
};

