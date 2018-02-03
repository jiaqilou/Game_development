#pragma once
#include "Actor.h"
#include "PaddleMove.h"
class Paddle :
	public Actor
{
public:
	Paddle(Game* game);
	void ActorInput(const Uint8* keyState) override;
	~Paddle();

private:
	//PaddleMove* pm;
};

