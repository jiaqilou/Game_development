#pragma once
#include "Actor.h"
#include "SDL/SDL.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);

	void ActorInput(const Uint8* keyState) override;

private:
	SpriteComponent* sc;
};