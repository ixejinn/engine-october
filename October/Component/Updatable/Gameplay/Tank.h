#pragma once
#include "../Updatable.h"
#include "../../Collidable.h"
#include "../../../Utils/MemoryPool.h"

#include "Bomb.h"
#include "Bullet.h"

class Tank : public Updatable, public Collidable
{
private:
	int life_{ 3 };

	MemoryPool<Bomb> bombs_;
	MemoryPool<Bullet> bullets_;

	Tank(GameObject* owner);

	void Update() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void OnCollision(Collision* collision) override;

	void ShowDetails() override;

	static Component* CreateComponent(GameObject* owner);
};