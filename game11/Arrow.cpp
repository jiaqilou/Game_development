#include "Arrow.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CollisionComponent.h"
#include <algorithm>


Arrow::Arrow( Game* game)
	:Actor (game)
{

	mMesh = new MeshComponent(this);
	mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));

	mScale = 0.15f;
}


Arrow::~Arrow()
{
}

void Arrow::UpdateActor(float deltaTime) {
	Vector3 p = mGame->first_check->GetPosition()- mGame->my_player->GetPosition();
	p.Normalize();
	Vector3 i = Vector3 (1, 0, 0);
	Vector3 axis = Vector3::Cross(i, p);
	axis.Normalize();
	float angle = Math::Acos(Vector3::Dot(p,i));
	Quaternion q = Quaternion(axis, angle);
	mQuat = q;

	SetPosition(mGame->GetRenderer()->Unproject(Vector3(0, 350, 0.1f)));
}