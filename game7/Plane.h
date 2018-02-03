#pragma once
#include "Actor.h"
#include "PlaneMove.h"
class Plane :
	public Actor
{
public:
	Plane(class Game* game);
	~Plane();
};

