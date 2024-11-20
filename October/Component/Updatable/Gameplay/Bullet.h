#pragma once
#include "../Updatable.h"
#include "../../Collidable.h"

class Transform;
class Rigidbody;

class Bullet : public Updatable, public Collidable
{
private:
	float speed_{ 10.f };
	int ricochet_{ 1 };

	Transform* trans_{ nullptr };
	Rigidbody* rb_{ nullptr };

	Bullet(GameObject* owner);

	void Update() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void OnCollision(Collision* collision) override;

	void ShowDetails() override;

	void SetBullet(float speed, int ricochet);

	static Component* CreateComponent(GameObject* owner);
};