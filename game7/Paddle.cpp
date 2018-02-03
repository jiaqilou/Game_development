#include "Paddle.h"
#include "Game.h"


Paddle::Paddle(Game* game)
	: Actor(game)
{
	mSprite = new SpriteComponent(this, 100);
	mMove = new PaddleMove(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Paddle.png"));

	mColl = new CollisionComponent(this);
	mColl->SetSize(104, 24);
}


Paddle::~Paddle()
{
}

void Paddle::ActorInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT]) {
		mMove->SetForwardSpeed(-250);
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		mMove->SetForwardSpeed(250);
	}
	if (!state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) {
		mMove->SetForwardSpeed(0);
	}

}