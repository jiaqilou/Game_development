#include "Player.h"
#include "Game.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"

Player::Player(Game* game)
	:Actor (game)
{
	mMove = new PlayerMove(this);

	mColl = new CollisionComponent(this);
	mColl->SetSize(50, 175, 50);

	mCamera = new CameraComponent(this);
}


Player::~Player()
{
}
