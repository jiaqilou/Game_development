#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(EActive)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
	,mSprite(NULL)
	,mMove(NULL)
	//,mPaddle(NULL)
	//,mBallmove(NULL)
	, mColl(NULL)
{
	// TODO
	mGame->AddActor(this);
}

Actor::~Actor()
{
	// TODO
	mGame->RemoveActor(this);
	delete mSprite;
	delete mMove;
	//delete mPaddle;
	delete mColl;
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
	

	/*if (mPaddle != NULL) {
		if (mState == EActive) {
			mPaddle->Update(deltaTime);
		}
	}*/
	if (mSprite != NULL) {
		if (mState == EActive) {
			mSprite->Update(deltaTime);
		}
	}

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
		if (mSprite != NULL) {
			if (mState == EActive) {
				mSprite->ProcessInput(keyState);
			}
		}

		if (mColl != NULL) {
			if (mState == EActive) {
				mColl->ProcessInput(keyState);
			}
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const Uint8* keyState)
{
}

SpriteComponent* Actor::GetSprite() {
	return mSprite;
}

void Actor::SetSprite(SpriteComponent* sprite) {
	mSprite = sprite;
}

Vector2 Actor::GetForward() {
	Vector2 direction;
	direction.x = cos(mRotation);
	direction.y = -(sin(mRotation));
	return direction;
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