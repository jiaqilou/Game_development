#pragma once
#include "Actor.h"
#include "Plane.h"
#include "Bullet.h"
class Tower :
	public Actor
{
public:
	Tower(class Game* game);
	~Tower();

	void UpdateActor(float deltaTime) override;

	Plane* FindCloestPlane();
private:
	float attact_timer;

	bool can_attack;
};

