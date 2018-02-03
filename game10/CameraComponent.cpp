#include "CameraComponent.h"
#include "Actor.h"
#include"Game.h"
#include "Renderer.h"
#include "Math.h"

CameraComponent::CameraComponent(Actor* owner)
	:Component(owner)
	,mPitchAngle(0.0)
	, mPitchSpeed(0.0)
{
}


CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime) {
	mPitchAngle = mPitchAngle+mPitchSpeed*deltaTime;
	//mPitchAngle = Math::PiOver4;
	if (mPitchAngle > Math::PiOver4 ) {
		mPitchAngle = Math::PiOver4;
	}
	else if (mPitchAngle < -Math::PiOver4) {
		mPitchAngle = -Math::PiOver4;
	}

	Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 combine = pitch*yaw;


	Vector3 eye = mOwner->GetPosition();
	Vector3 forward_vector = Vector3::Transform(Vector3(1, 0, 0), combine);
	Vector3 target = forward_vector + mOwner->GetPosition();	

	Matrix4 ma = Matrix4::CreateLookAt(eye, target, Vector3(0,0,1));
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(ma);
}

float CameraComponent::GetPitchSpeed() {
	return mPitchSpeed;
}

void CameraComponent::SetPitchSpeed(float value) {
	mPitchSpeed = value;
}