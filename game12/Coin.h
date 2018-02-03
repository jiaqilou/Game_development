#pragma once
#include "Actor.h"

class Coin :
	public Actor
{
public:
	Coin(class Game* game);
	~Coin();

	void UpdateActor(float deltaTime) override;
};

