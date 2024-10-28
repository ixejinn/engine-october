#include "Player.h"

//#include <GLFW/glfw3.h>
#include "../../LateUpdatable/Sprite.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Utils/imgui/imgui.h"

Player::Player(GameObject* owner) : Component(owner)
{
	sp_ = static_cast<Sprite*>(owner_->GetComponent(typeid(Sprite)));
	owner_->SetCollidable(static_cast<Collidable*>(this));
}

void Player::Update()
{
	if (sp_ == nullptr)
		sp_ = static_cast<Sprite*>(owner_->GetComponent(typeid(Sprite)));
}

void Player::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");
	if (compData != data.end())
	{

	}
}

json Player::SaveToJson()
{
	json data, compData;
	data["type"] = typeid(Player).name();

	data["compData"] = compData;
	return data;
}

void Player::OnCollision(Collision* collision)
{
	static double timeValue = -10.0;
	timeValue += 10;
	float colorValue = float(sin(timeValue)) / 2.0f + 0.5f;
	sp_->SetColor(0, { colorValue, colorValue, colorValue });
	sp_->SetColor(1, { colorValue, 0, 0 });
	sp_->SetColor(2, { 0, colorValue, 0 });
	sp_->SetColor(3, { 0, 0, colorValue });
}

void Player::ShowDetails()
{
	ImGui::SeparatorText("Player");
}

Component* Player::CreateComponent(GameObject* owner)
{
	Player* newComp = new Player(owner);
	return static_cast<Component*>(newComp);
}
