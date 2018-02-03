#pragma once
#include "Actor.h"
class Bullet :
	public Actor
{
public:
	Bullet(class Game* game);
	~Bullet();

	void UpdateActor(float deltaTime) override;

	float life;
};

