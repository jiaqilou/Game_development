#include "Block.h"
#include "Game.h"


Block::Block(Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this, 100);

	mColl = new CollisionComponent(this);
	mColl->SetSize(64, 32);

	mGame->addBlock(this);
}


Block::~Block()
{
	mGame->removeBlock(this);
}

void Block::set_texture_of_sc(SDL_Texture* texture) {
	if (texture != NULL) {
		mSprite->SetTexture(texture);
	}
}

