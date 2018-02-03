#pragma once
#include "Component.h"
class CameraComponent :
	public Component
{
public:
	CameraComponent(class Actor* owner);
	~CameraComponent();

	void Update(float deltaTime) override;

	float GetPitchSpeed();

	void SetPitchSpeed(float value);
private:
	float mPitchAngle;
	float mPitchSpeed;
};

