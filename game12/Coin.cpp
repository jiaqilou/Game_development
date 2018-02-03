#include "Coin.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

Coin::Coin(Game* game)
	:Actor(game)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));

	mColl = new CollisionComponent(this);
	mColl->SetSize(100, 100, 100);

}


Coin::~Coin()
{
}

void Coin::UpdateActor(float deltaTime) {
	SetRotation(GetRotation()+Math::Pi*deltaTime );

	if (getColl()->Intersect(mGame->my_player->getColl())) {
		Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Coin.wav"), 0);
		SetState(Actor::EDead);
		mGame->mHud->coin++;
		//dead = true;
	}
}
