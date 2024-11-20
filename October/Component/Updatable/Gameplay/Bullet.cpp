#include "Bullet.h"

#include "../../Transform.h"
#include "../../Rigidbody.h"
#include "../../FixedUpdatable/CircleCollider.h"
#include "../../LateUpdatable/Sprite.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Utils/imgui/imgui.h"

Bullet::Bullet(GameObject* owner) : Updatable(owner)
{
	if (owner_->GetComponentNum() >= 1)
		owner_->InitializeComponent();

	owner_->AddComponent(typeid(Rigidbody));
	owner_->AddComponent(typeid(CircleCollider));
	owner_->AddComponent(typeid(Sprite));

	trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));
	rb_ = static_cast<Rigidbody*>(owner_->GetComponent(typeid(Rigidbody)));

	// 텍스처 넣어줘야 함 2 종류 쓸지도?
}

void Bullet::Update()
{
}

void Bullet::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");
	if (compData != data.end())
	{
		auto it = compData->find("speed");
		speed_ = it.value();

		it = compData->find("ricochet");
		ricochet_ = it.value();
	}
}

json Bullet::SaveToJson()
{
	json data, compData;
	data["type"] = typeid(Bullet).name();

	compData["speed"] = speed_;
	compData["ricochet"] = ricochet_;

	data["compData"] = compData;
	return data;
}

void Bullet::OnCollision(Collision* collision)
{
	// 벽이면 부딪힌 횟수 증가, ricochet 보다 커지면 없어짐
	// 탱크 또는 폭탄이면 없어짐
}

void Bullet::ShowDetails()
{
	ImGui::SeparatorText("Bullet");
}

void Bullet::SetBullet(float speed, int ricochet)
{
	speed_ = speed;
	ricochet_ = ricochet;
}

Component* Bullet::CreateComponent(GameObject* owner)
{
	Bullet* newComp = new Bullet(owner);
	return static_cast<Component*>(newComp);
}
