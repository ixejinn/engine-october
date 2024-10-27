#include "Player.h"

#include "../../LateUpdatable/Sprite.h"
#include "../../../GameObject/GameObject.h"

Player::Player(GameObject* owner) : Component(owner)
{
	sp_ = static_cast<Sprite*>(owner_->GetComponent(typeid(Sprite)));
}

void Player::Update()
{
}
