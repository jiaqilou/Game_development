#include "Coin.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include <SDL/SDL_stdinc.h>

Coin::Coin(Game* game) 
	:Actor(game)
{
	mSprite = new AnimatedSprite(this, 100);
	//mAnimated = new AnimatedSprite(this, 100);
	mColl = new CollisionComponent(this);
	mColl->SetSize(32, 32);


}


Coin::~Coin()
{
}

void Coin::set_texture_of_sc(SDL_Texture* texture) {
	if (texture != NULL) {
		mSprite->SetTexture(texture);
	}
}

void Coin::UpdateActor(float deltaTime) {
	if (mColl->Intersect(mGame->mPlayer->getColl())) {
		if (Mix_PlayChannel(-1, mGame->GetSound("Assets/Coin/coin.wav"), 0) == -1) {
			printf("Mix_PlayChannel: %s\n", Mix_GetError());
		}
		this->mState = EDead;
	}
}