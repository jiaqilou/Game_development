#include "Turret.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MoveComponent.h"

Turret::Turret(Game* game)
	:Actor (game)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/TankTurret.gpmesh"));
	mScale = 1.8;
	mMove = new MoveComponent(this);

	mLeftKey = SDL_SCANCODE_Q;
	mRightKey = SDL_SCANCODE_E;
}


Turret::~Turret()
{
}

void Turret::ActorInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[mLeftKey]) {
		this->mMove->SetAngularSpeed(Math::TwoPi);
	}
	if (state[mRightKey]) {
		this->mMove->SetAngularSpeed(-Math::TwoPi);
	}
	if (!state[mLeftKey] && !state[mRightKey]) {
		this->mMove->SetAngularSpeed(0);
	}
}

void Turret::SetPlayerTwo() {
	mLeftKey = SDL_SCANCODE_I;
	mRightKey = SDL_SCANCODE_P;

	mMesh->SetTextureIndex(1);
}