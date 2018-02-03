#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	// TODO: Implement
	if (this->GetMax().x < other->GetMin().x) {
		return false;
	}
	else if (this->GetMin().x > other->GetMax().x) {
		return false;
	}
	else if (this->GetMax().y < other->GetMin().y) {
		return false;
	}
	else if (this->GetMin().y > other->GetMax().y) {
		return false;
	}

	return true;
}

Vector2 CollisionComponent::GetMin() const
{
	// TODO: Implement
	Vector2 min;
	min.x = mOwner->GetPosition().x - (mWidth*mOwner->GetScale()) / 2;
	min.y = mOwner->GetPosition().y - (mHeight*mOwner->GetScale()) / 2;
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	// TODO: Implement
	Vector2 max;
	max.x = mOwner->GetPosition().x + (mWidth*mOwner->GetScale()) / 2;
	max.y = mOwner->GetPosition().y + (mHeight*mOwner->GetScale()) / 2;
	return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

