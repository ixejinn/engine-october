#include "Wall.h"

#include "../../../GameObject/GameObject.h"
#include "../../Rigidbody.h"
#include "../../LateUpdatable/Sprite.h"
#include "../PlayerController.h"
#include "../../FixedUpdatable/BoxCollider.h"
#include "../../../Utils/imgui/imgui.h"

BaseWall::BaseWall(GameObject* owner) : Component(owner)
{
	if (owner_->GetComponentNum() >= 1)
		owner_->InitializeComponent();

	owner_->AddComponent(typeid(Sprite));
	owner_->AddComponent(typeid(Rigidbody));
	owner_->AddComponent(typeid(BoxCollider));
	owner_->AddComponent(typeid(PlayerController));
}

void BaseWall::Update()
{
}

void BaseWall::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");
	if (compData != data.end())
	{ }
}

json BaseWall::SaveToJson()
{
	json data, compData;
	data["type"] = typeid(BoxCollider).name();

	data["compData"] = compData;
	return data;
}

Wall::Wall(GameObject* owner) : BaseWall(owner) {}

void Wall::ShowDetails()
{
	ImGui::SeparatorText("Wall");
}

Component* Wall::CreateComponent(GameObject* owner)
{
	Wall* newComp = new Wall(owner);
	return static_cast<Wall*>(newComp);
}

BreakableWall::BreakableWall(GameObject* owner) : BaseWall(owner)
{
	owner_->SetCollidable(static_cast<Collidable*>(this));
}

void BreakableWall::OnCollision(Collision* collision)
{
	// ÆøÅº
}

void BreakableWall::ShowDetails()
{
	ImGui::SeparatorText("BreakableWall");
}

Component* BreakableWall::CreateComponent(GameObject* owner)
{
	BreakableWall* newComp = new BreakableWall(owner);
	return static_cast<BreakableWall*>(newComp);
}
