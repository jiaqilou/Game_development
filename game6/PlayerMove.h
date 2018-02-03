#pragma once
#include "MoveComponent.h"
class PlayerMove :
	public MoveComponent
{
public:
	PlayerMove(class Actor* owner);
	~PlayerMove();

	void ProcessInput(const Uint8* keyState) override;

	void Update(float deltaTime) override;
private:
	float mYSpeed;

	bool mSpacePressed;

	bool mInAir;

	const Uint8 *state;
	Uint32 timeout;

	bool timeup;

	float holdingTime;
};

