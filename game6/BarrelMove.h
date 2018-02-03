#pragma once
#include "MoveComponent.h"
#include "Math.h"
class BarrelMove :
	public MoveComponent
{
public:
	BarrelMove(class Actor* owner);
	~BarrelMove();

	void Update(float deltaTime) override;

private:
	int mYSpeed;
};

