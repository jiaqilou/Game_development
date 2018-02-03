#pragma once
#include "Actor.h"
class Turret :
	public Actor
{
public:
	Turret(class Game* game);
	~Turret();
	void ActorInput(const Uint8* keyState) override;

	void SetPlayerTwo();
private:
	int mLeftKey;
	int mRightKey;
};

