#pragma once
#include "MoveComponent.h"
class TankMove :
	public MoveComponent
{
public:
	TankMove(class Actor* owner);
	~TankMove();

	void Update(float deltaTime) override;

	void ProcessInput(const Uint8* keyState) override;

	void SetPlayerTwo();
private:
	int mForwardKey;
	int mBackKey;
	int mLeftKey;
	int mRightKey;
};

