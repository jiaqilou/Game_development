#pragma once
#include "Actor.h"
#include <SDL/SDL_image.h>
#include "SDL/SDL.h"'
class Coin :
	public Actor
{
public:
	Coin(Game* mOwner);
	~Coin();
	void UpdateActor(float deltaTime) override;
	void set_texture_of_sc(SDL_Texture* texture);

	//AnimatedSprite* mAnimated;
};

