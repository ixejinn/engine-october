#pragma once
#include "../Updatable.h"
#include "../../Collidable.h"

class Sprite;
struct Collision;

class Player : public Updatable, public Collidable
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