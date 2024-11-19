#include "Bomb.h"

#include "../../Transform.h"
#include "../../Rigidbody.h"
#include "../../LateUpdatable/Sprite.h"
#include "../../FixedUpdatable/CircleCollider.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Utils/imgui/imgui.h"

Bomb::Bomb(GameObject* owner) : Updatable(owner)
{
	if (owner_->GetComponentNum() >= 1)
		owner_->InitializeComponent();

	owner_->AddComponent(typeid(Sprite));
	owner_->AddComponent(typeid(Rigidbody));
	owner_->AddComponent(typeid(CircleCollider));

	trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));
	sp_ = static_cast<Sprite*>(owner_->GetComponent(typeid(Sprite)));
	col_ = static_cast<CircleCollider*>(owner_->GetComponent(typeid(CircleCollider)));
	col_->SetTrigger(false);

	//owner_->active_ = false;
	Activate();	// 나중에 빼기
}

void Bomb::Update()
{
	auto now = std::chrono::system_clock::now();
	auto test = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count();

	switch (state_)
	{
	case BOMB:
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count() > delayBeforeExplosion_)
			SetExplosion();
		break;

	case EXPLOSION:
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count() > explosionTime_)
			Inactivate();
		break;
	}
}

void Bomb::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");
	if (compData != data.end())
	{
		auto it = compData->find("delayBeforeExplosion");
		delayBeforeExplosion_ = it.value();

		it = compData->find("explosionTime");
		explosionTime_ = it.value();

		it = compData->find("explosionScale");
		explosionScale_ = it.value();
	}
}

json Bomb::SaveToJson()
{
	json data, compData;
	data["type"] = typeid(Bomb).name();

	compData["delayBeforeExplosion"] = delayBeforeExplosion_;
	compData["explosionTime"] = explosionTime_;
	compData["explosionScale"] = explosionScale_;

	data["compData"] = compData;
	return data;
}

void Bomb::OnCollision(Collision* collision)
{
	if (state_ == BOMB)
	{
		// bullet 맞으면 바로 터지게
	}
}

void Bomb::ShowDetails()
{
	ImGui::SeparatorText("Bomb");
}

void Bomb::Activate()
{
	owner_->active_ = true;

	state_ = BOMB;
	start_ = std::chrono::system_clock::now();

	sp_->SetAlpha(1.f);
	col_->SetTrigger(false);
}

void Bomb::SetExplosion()
{
	state_ = EXPLOSION;
	start_ = std::chrono::system_clock::now();

	trans_->SetScale(trans_->GetScale() * explosionScale_);
	sp_->SetAlpha(0.8f);
	col_->SetTrigger(true);
}

void Bomb::Inactivate()
{
	owner_->active_ = false;

	trans_->SetScale(trans_->GetScale() / explosionScale_);
	// Release memory pool
}

Component* Bomb::CreateComponent(GameObject* owner)
{
	Bomb* newComp = new Bomb(owner);
	return static_cast<Component*>(newComp);
}
