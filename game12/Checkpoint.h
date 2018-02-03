#pragma once
#include "Actor.h"

class Checkpoint :
	public Actor
{
public:
	Checkpoint(class Game* game);
	~Checkpoint();

	void UpdateActor(float deltaTime) override;
	bool active;

	void setString(std::string s);
	std::string mLevelString;

	std::string checkpointtext;
private:
	//std::string mLevelString;
};


