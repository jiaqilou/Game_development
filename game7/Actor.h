#pragma once
#include <vector>
#include <SDL/SDL_stdinc.h>
#include <cmath>
#include "Math.h"


class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	
	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState);
	// Any actor-specific update code (overridable)
	virtual void ActorInput(const Uint8* keyState);

	// Getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	class SpriteComponent* GetSprite();
	void SetSprite(class SpriteComponent* sprite);

	Vector2 GetForward();

	class MoveComponent* GetMove();
	void SetMove(class MoveComponent* move);

	class CollisionComponent* getColl();
	void setColl(class CollisionComponent* Coll);

protected:
	class Game* mGame;
	// Actor's state
	State mState;
	// Transform
	Vector2 mPosition;
	float mScale;
	float mRotation;
	class SpriteComponent* mSprite;
	class MoveComponent* mMove;
	//PaddleMove* mPaddle;
	//BallMove* mBallmove;
	class CollisionComponent* mColl;
};
