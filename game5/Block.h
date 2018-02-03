#pragma once
#include "Actor.h"
#include <SDL/SDL_image.h>
class Block :public Actor
{
public:
	Block(Game* game);
	~Block();

	void set_texture_of_sc(SDL_Texture* texture);

private:
	
};

