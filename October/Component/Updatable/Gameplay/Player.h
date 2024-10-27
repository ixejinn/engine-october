#pragma once
#include "../../Component.h"
#include "../../Collidable.h"
#include "../Updatable.h"

class Sprite;
class Collision;

class Player : public Component, public Collidable, public Updatable
{
private:
	Sprite* sp_{ nullptr };

	Player(GameObject* owner);

	void Update() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void OnCollision(Collision* collision) override;

	void ShowDetails() override;

	static Component* CreateComponent(GameObject* owner);
};