#pragma once
#include <chrono>
#include "../Updatable.h"
#include "../../Collidable.h"

class Transform;
class Sprite;
class CircleCollider;

class Bomb : public Updatable, public Collidable
{
private:
	std::chrono::system_clock::time_point start_{};
	int delayBeforeExplosion_{ 5000 };	// milliseconds
	int explosionTime_{ 1000 };

	enum BombState{ BOMB, EXPLOSION };
	BombState state_{ BOMB };

	float explosionScale_{ 1.5f };	// Relative with Transform's scale

	Transform* trans_{ nullptr };
	Sprite* sp_{ nullptr };
	CircleCollider* col_{ nullptr };

	Bomb(GameObject* owner);

	void Update() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void OnCollision(Collision* collision) override;

	void ShowDetails() override;

	void Activate();
	void SetExplosion();
	void Inactivate();

	static Component* CreateComponent(GameObject* owner);
};

