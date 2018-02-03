#include "Tower.h"
#include "Game.h"


Tower::Tower(Game* game)
	:Actor (game)
	,attact_timer(0.0)
	, can_attack(true)
{
	mSprite = new SpriteComponent(this, 200);
	mSprite->SetTexture(mGame->GetTexture("Assets/Tower.png"));
}


Tower::~Tower()
{
}

void Tower::UpdateActor(float deltaTime) {
	
	if (!can_attack) {
		attact_timer += deltaTime;
		if (attact_timer >= 2) {
			can_attack = true;
			attact_timer = 0;
		}
	}

	if (can_attack) {
		
		Plane* target = FindCloestPlane();
		if (target != NULL) {
			if (std::sqrt(std::pow((target->GetPosition().x - GetPosition().x),2) + std::pow((target->GetPosition().y - GetPosition().y),2)) < 100) {
				this->SetRotation(Math::Atan2(-(target->GetPosition().y - GetPosition().y), (target->GetPosition().x - GetPosition().x)));
				Bullet* bullet = new Bullet(mGame);
				bullet->SetPosition(GetPosition());
				bullet->SetRotation(GetRotation());
			}
		}
		can_attack = false;
	}

}

Plane* Tower::FindCloestPlane() {
	std::vector <Plane*> list = mGame->mPlane_vector;
	if (!list.empty()) {
		Plane* target = list[0];
		float shortest = std::abs(target->GetPosition().x - GetPosition().x) + std::abs(target->GetPosition().y - GetPosition().y);
		for (Plane* p : list) {
			float dis = std::abs(p->GetPosition().x - GetPosition().x) + std::abs(p->GetPosition().y - GetPosition().y);
			if (dis < shortest) {
				shortest = dis;
				target = p;
			}
		}
		return target;
	}
	else {
		return NULL;
	}
	
}
