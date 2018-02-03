#include "AnimatedSprite.h"



AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent( owner,drawOrder)
{

	mAnimTimer = 0.0;

	mAnimSpeed = 15.0;
}


AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::AddImage(SDL_Texture* image) {
	if (image != NULL)
		mImages.push_back(image);
}

void AnimatedSprite::Update(float deltaTime) {
	mAnimTimer += mAnimSpeed * deltaTime;
	int currentframe = (int)mAnimTimer;

	int im = currentframe % mImages.size();
	SetTexture(mImages[im]);

}