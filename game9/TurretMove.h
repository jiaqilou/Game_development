#pragma once
#include "MoveComponent.h"
class TurretMove :
	public MoveComponent
{
public:
	TurretMove(class Actor* owner);
	~TurretMove();
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
};

