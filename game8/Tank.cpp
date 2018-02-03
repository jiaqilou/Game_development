#include "Tank.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "TankMove.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

Tank::Tank(Game* game):
	Actor (game)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/TankBase.gpmesh"));

	mMove = new TankMove(this);

	mTurret = new Turret(mGame);

	mColl = new CollisionComponent(this);
	mColl->SetSize(30, 30, 30);

	firing_key = SDL_SCANCODE_LSHIFT;
	leading_fire = true;
}


Tank::~Tank()
{
}

void Tank::UpdateActor(float deltaTime) {
	mTurret->SetPosition(GetPosition());
}

void Tank::SetPlayerTwo() {
	TankMove* t =(TankMove*)(mMove);
	t->SetPlayerTwo();
	mTurret->SetPlayerTwo();
	mMesh->SetTextureIndex(1);

	firing_key = SDL_SCANCODE_RSHIFT;
}

void Tank::Respawn() {
	SetPosition(origin_pos);
	SetRotation(0);
}

void Tank::Fire() {
	Bullet* bullet = new Bullet(mGame);
	bullet->set_tank(this); 
	bullet->SetRotation(mTurret->GetRotation());
	bullet->SetPosition(GetPosition());
}

void Tank::ActorInput(const Uint8* keyState) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	
	if (state[firing_key] ) {
		if (leading_fire) {
			Fire();
			leading_fire = false;
		}
	}
	if (!state[firing_key]) {
		leading_fire = true;
	}
}