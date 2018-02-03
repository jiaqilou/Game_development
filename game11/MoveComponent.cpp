#include "MoveComponent.h"
#include "Actor.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
,mStrafeSpeed(0.0F)
{
	
}

MoveComponent::~MoveComponent() {
	printf("paretns des \n");
}

void MoveComponent::Update(float deltaTime)
{
	/*mOwner->SetRotation(mOwner->GetRotation()+  deltaTime*mAngularSpeed);
	Vector3 new_pos;
	new_pos.x = mOwner->GetForward().x * mForwardSpeed * deltaTime + mOwner->GetPosition().x;
	new_pos.y = mOwner->GetForward().y * mForwardSpeed * deltaTime + mOwner->GetPosition().y;
	new_pos.z = mOwner->GetForward().z * mForwardSpeed * deltaTime + mOwner->GetPosition().z;
	mOwner->SetPosition(new_pos);

	Vector3 nn_pos;
	nn_pos.x = mOwner->GetRight().x * mStrafeSpeed * deltaTime + mOwner->GetPosition().x;
	nn_pos.y = mOwner->GetRight().y * mStrafeSpeed * deltaTime + mOwner->GetPosition().y;
	nn_pos.z = mOwner->GetRight().z * mStrafeSpeed * deltaTime + mOwner->GetPosition().z;
	mOwner->SetPosition(nn_pos);*/
}
