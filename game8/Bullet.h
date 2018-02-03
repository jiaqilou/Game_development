#pragma once
#include "Actor.h"


class Bullet :
	public Actor
{
public:
	Bullet(class Game* game);
	~Bullet();

	void set_tank(class Tank* tank);
	void UpdateActor(float deltaTime) override;
private:
	class Tank* mTank;
};

