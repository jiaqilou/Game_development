#include "MoveComponent.h"
#include "Actor.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	mOwner->SetRotation(mOwner->GetRotation()+  deltaTime*mAngularSpeed);
	Vector3 new_pos;
	new_pos.x = mOwner->GetForward().x * mForwardSpeed * deltaTime + mOwner->GetPosition().x;
	new_pos.y = mOwner->GetForward().y * mForwardSpeed * deltaTime + mOwner->GetPosition().y;
	new_pos.z = mOwner->GetForward().z * mForwardSpeed * deltaTime + mOwner->GetPosition().z;
	mOwner->SetPosition(new_pos);
}
