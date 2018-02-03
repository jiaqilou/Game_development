#pragma once
#include "Actor.h"
class Player :
	public Actor
{
public:
	Player(class Game* game);
	~Player();
	Vector3 respawn_position;

};

