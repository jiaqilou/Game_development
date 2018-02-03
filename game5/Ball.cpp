#include "Ball.h"
#include "Game.h"


Ball::Ball(Game* game) :Actor(game)
{
	mSprite = new SpriteComponent(this, 100);
	mSprite->SetTexture(mGame->GetTexture("Assets/Ball.png"));

	mMove = new BallMove(this);

	mColl = new CollisionComponent(this);
	mColl->SetSize(10,10);
}


Ball::~Ball()
{
}
