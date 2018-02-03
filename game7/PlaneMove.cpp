#include "PlaneMove.h"
#include "Game.h"
#include "Tile.h"

PlaneMove::PlaneMove(Actor* owner)
	:MoveComponent(owner)
{
	SetForwardSpeed(200);
	SetNextTile(mOwner->GetGame()->mGrid->GetStartTile()->GetParent());
}


PlaneMove::~PlaneMove()
{
}

void PlaneMove::Update(float deltaTime) {
	MoveComponent::Update(deltaTime);
	if (mNextTile != NULL) {
		float dis = std::abs(mOwner->GetPosition().x - mNextTile->GetPosition().x) +
			std::abs(mOwner->GetPosition().y - mNextTile->GetPosition().y);
		//printf("distance is %f \n", dis);
		if (dis < 2) {
			SetNextTile(mNextTile->GetParent());
		}
	}

	if (mOwner->GetPosition().x > 1086) {
		mOwner->SetState(Actor::EDead);
	}
}

void PlaneMove::SetNextTile(const Tile* tile) {
	mNextTile = tile;
	if (mNextTile == NULL) {
		mOwner->SetRotation(0);
	}
	else {
		mOwner->SetRotation(Math::Atan2(-(mNextTile->GetPosition().y - mOwner->GetPosition().y), mNextTile->GetPosition().x - mOwner->GetPosition().x));
	}
}