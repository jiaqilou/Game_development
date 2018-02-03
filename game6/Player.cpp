#include "Player.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"

Player::Player(Game* game)
	:Actor(game)
{
	mSprite = new AnimatedSprite(this,200);
	//mSprite->SetTexture(mGame->GetTexture("Assets/Player/Idle.png"));
	//SetSprite(mSprite);

	mColl = new CollisionComponent(this);
	mColl->SetSize(20, 64);

	mMove = new PlayerMove(this);
}


Player::~Player()
{
}
