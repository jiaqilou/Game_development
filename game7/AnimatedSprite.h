#pragma once
#include "SpriteComponent.h"
#include <vector>
class AnimatedSprite :
	public SpriteComponent
{
public:
	AnimatedSprite(Actor* owner, int drawOrder);
	~AnimatedSprite();

	std::vector <SDL_Texture*> mImages;

	float mAnimTimer;

	float mAnimSpeed;

	void AddImage(SDL_Texture* image) override;
	void Update(float deltaTime) override;


};

