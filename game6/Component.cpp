#include "Component.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

Component::Component(Actor* owner)
	:mOwner(owner)
{
}

Component::~Component()
{
}

void Component::Update(float deltaTime)
{
}

void Component::ProcessInput(const Uint8 *keyState)
{
}
