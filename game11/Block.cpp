#include "Block.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CollisionComponent.h"
#include <algorithm>
Block::Block(Game* game)
	:Actor(game)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mScale = 64;
	mColl = new CollisionComponent(this);
	mColl->SetSize(1, 1, 1);

	mGame->block_vector.push_back(this);
}


Block::~Block()
{
	std::vector <Block*> vec = mGame->block_vector;
	vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
}
