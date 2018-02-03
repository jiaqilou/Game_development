#pragma once
#include "Actor.h"
#include "Turret.h"
#include "Bullet.h"
class Tank :
	public Actor
{
public:
	Tank(class Game* game);
	~Tank();

	void UpdateActor(float deltaTime) override;

	void SetPlayerTwo();

	void Respawn();

	void Fire();

	void ActorInput(const Uint8* keyState) override;

	Vector3 origin_pos;
private:
	Turret* mTurret;

	

	int firing_key;

	bool leading_fire;
};

