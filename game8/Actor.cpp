#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include <algorithm>
#include "Math.h"

Actor::Actor(Game* game)
	:mGame(game)
	,mState(EActive)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
	,mMove(NULL)
	//,mPaddle(NULL)
	//,mBallmove(NULL)
	,mColl(NULL)
	,mMesh(NULL)
{
	// TODO
	mGame->AddActor(this);
}

Actor::~Actor()
{
	// TODO
	mGame->RemoveActor(this);
	delete mMove;
	//delete mPaddle;
	delete mColl;
	delete mMesh;
	//delete mBallmove;
}

void Actor::Update(float deltaTime)
{
	// TODO
	/*
	update all relevant components
	*/
	if (mColl != NULL) {
		if (mState == EActive) {
			mColl->Update(deltaTime);
		}
	}
	if (mMove != NULL) {
		if (mState == EActive) {
			mMove->Update(deltaTime);
		}
	}
	if (mMesh != NULL) {
		if (mState == EActive) {
			mMesh->Update(deltaTime);
		}
	}
	Matrix4 mScaleMatrix  ;
	mScaleMatrix= Matrix4::CreateScale(mScale);
	Matrix4 mRotationMatrix;
	mRotationMatrix= Matrix4::CreateRotationZ(mRotation);
	Matrix4 mPositionMatrix ;
	mPositionMatrix = Matrix4::CreateTranslation(mPosition);

	mWorldTransform = mScaleMatrix*mRotationMatrix*mPositionMatrix ;
	//mWorldTransform = *(mPositionMatrix);
	/*if (mBallmove != NULL) {
		if (mState == EActive) {
			mBallmove->Update(deltaTime);
		}
	}*/


	UpdateActor(deltaTime);
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	// TODO
	if (this->mState == EActive) {
		/*
		call processinput on all relevant components*/
		if (mMove != NULL) {
			if (mState == EActive) {
				mMove->ProcessInput(keyState);
			}
		}

		if (mColl != NULL) {
			if (mState == EActive) {
				mColl->ProcessInput(keyState);
			}
		}

		if (mMesh != NULL) {
			if (mState == EActive) {
				mMesh->ProcessInput(keyState);
			}
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const Uint8* keyState)
{
}

Vector3 Actor::GetForward() {
	return Vector3(Math::Cos(mRotation), Math::Sin(mRotation), 0.0f);
}

MoveComponent* Actor::GetMove() {
	return mMove;
}

void Actor::SetMove(MoveComponent* move) {
	mMove = move;
}

CollisionComponent* Actor::getColl() {
	return mColl;
}
void Actor::setColl(CollisionComponent* Coll) {
	if (Coll != NULL) {
		mColl = Coll;
	}
}