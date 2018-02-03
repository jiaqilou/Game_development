#include "Ship.h"
#include "Game.h"

Ship::Ship(class Game* game) 
: Actor(game)
{
	sc = new SpriteComponent(this);
	sc->SetTexture(mGame->GetTexture("Assets/Ship.png"));
	SetSprite(sc);

	MoveComponent* mc = new MoveComponent(this);
	SetMove(mc);
}

void Ship::ActorInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	const Uint8 *old_state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_UP]) {
		GetMove()->SetForwardSpeed(500);
		sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	if (state[SDL_SCANCODE_DOWN]) {
		GetMove()->SetForwardSpeed(-500);
		sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	if (state[SDL_SCANCODE_LEFT]) {
		GetMove()->SetAngularSpeed(2.5);
		sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		GetMove()->SetAngularSpeed(-2.5);
		sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	if (!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
		GetMove()->SetForwardSpeed(0);
	}
	if (!state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) {
		GetMove()->SetAngularSpeed(0);		
	}
	if (!state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
		sc->SetTexture(mGame->GetTexture("Assets/Ship.png"));
	}
}