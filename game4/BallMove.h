#pragma once
#include "MoveComponent.h"
#include "Math.h"
class BallMove :
	public MoveComponent
{
public:
	BallMove(class Actor* owner);
	~BallMove();
	void Update(float deltaTime) override;

private:
	Vector2 mBallVelocity;
};

