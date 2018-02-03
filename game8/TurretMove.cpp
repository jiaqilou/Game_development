#include "TurretMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"



TurretMove::TurretMove(class Actor* owner)
	:MoveComponent(owner)
{
}


TurretMove::~TurretMove()
{
}

void TurretMove::Update(float deltaTime) {
	MoveComponent::Update(deltaTime);
}


void TurretMove::ProcessInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_Q]) {
		this->SetAngularSpeed(Math::TwoPi);
	}
	if (state[SDL_SCANCODE_E]) {
		this->SetAngularSpeed(-Math::TwoPi);
	}
	if (!state[SDL_SCANCODE_Q] && !state[SDL_SCANCODE_E]) {
		this->SetAngularSpeed(0);
	}
}