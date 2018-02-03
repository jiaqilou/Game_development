#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"


PlayerMove::PlayerMove(class Actor* owner)
	:MoveComponent(owner)
{
	mYSpeed = 0.0;
	mSpacePressed = false;
	mInAir = false;
}


PlayerMove::~PlayerMove()
{
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	const Uint8 *old_state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT]) {
		SetForwardSpeed(-300);
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		SetForwardSpeed(300);
	}
	if (!state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) {
		SetForwardSpeed(0);
	}
	if (state[SDL_SCANCODE_SPACE]) {
		if (!mSpacePressed && !mInAir) {
			mYSpeed = -800;
			mOwner->SetPosition(Vector2(mOwner->GetPosition().x,mOwner->GetPosition().y-1));
			mInAir = true;


			// play sample on first free unreserved channel
			// play it exactly once through
			// Mix_Chunk *sample; //previously loaded
			if (Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Player/Jump.wav"), 0) == -1) {
				printf("Mix_PlayChannel: %s\n", Mix_GetError());
				// may be critical error, or maybe just no channels were free.
				// you could allocated another channel in that case...
			}
		}
		mSpacePressed = true;
	}
	else mSpacePressed = false;

}

void PlayerMove::Update(float deltaTime) {
	int falling = 2000;

	Vector2 new_pos;
	new_pos.x = mOwner->GetForward().x * mForwardSpeed * deltaTime + mOwner->GetPosition().x;
	new_pos.y = mYSpeed*deltaTime + mOwner->GetPosition().y;
	if (new_pos.y >= 768) {
		new_pos.y = 768;
		mInAir = false;
	}

	for (Block* i : mOwner->GetGame()->get_block_vector()) {
		if (mOwner->getColl()->Intersect(i->getColl())) {
			int dx1, dx2, dy1, dy2;
			dy1 = abs(i->GetPosition().y - 16 - (mOwner->GetPosition().y + 32));
			dy2 = abs(mOwner->GetPosition().y - 32 - (i->GetPosition().y + 16));
			dx1 = abs(mOwner->GetPosition().x+16 - i->GetPosition().x+32);
			dx2 = abs(i->GetPosition().x + 32 - (mOwner->GetPosition().x - 16));
			
			
			if (std::min(dy1, dy2) < std::min(dx1, dx2)) {
				if (dy1 < dy2) {
					new_pos.y = new_pos.y - dy1;
					falling = 0;
					mInAir = false;
					mYSpeed = 0;
				}else{ 
					new_pos.y = new_pos.y + dy2;
					if (mYSpeed < 0) mYSpeed = 0;
				}
						
			}
			else {
				if (dx1 < dx2) {
					new_pos.x = new_pos.x - dx1;
				}
				else {
					new_pos.x = new_pos.x + dx2;
				}
			}

		}
	}
	
	
	mOwner->SetPosition(new_pos);
	float times = 1 / deltaTime;
	mYSpeed = mYSpeed + falling / times;
}
