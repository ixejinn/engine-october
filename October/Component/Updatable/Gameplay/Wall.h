#pragma once
#include "../../Collidable.h"
#include "../Updatable.h"

class BaseWall : public Updatable
{
protected:
	BaseWall(GameObject* owner);

	void Update() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;
};

class Wall : public BaseWall
{
private:
	Wall(GameObject* owner);

public:
	void ShowDetails() override;

	static Component* CreateComponent(GameObject* owner);
};

class BreakableWall : public BaseWall, public Collidable
{
private:
	BreakableWall(GameObject* owner);

public:
	void OnCollision(Collision* collision) override;

	void ShowDetails() override;

	static Component* CreateComponent(GameObject* owner);
};