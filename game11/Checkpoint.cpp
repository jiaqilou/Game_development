#include "Checkpoint.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CollisionComponent.h"
#include <algorithm>

Checkpoint::Checkpoint(Game* game)
	:Actor(game)
{
	active = false;
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));

	mColl = new CollisionComponent(this);
	mColl->SetSize(25, 25, 25);
}


Checkpoint::~Checkpoint()
{
}

void Checkpoint::UpdateActor(float deltaTime) {
	if (active) {
		mMesh->SetTextureIndex(0);
	}
	else {
		mMesh->SetTextureIndex(1);
	}
}

void Checkpoint::setString(std::string s) {
	mLevelString = s;
}