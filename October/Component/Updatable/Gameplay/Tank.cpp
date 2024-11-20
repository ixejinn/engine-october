#include "Tank.h"

Tank::Tank(GameObject* owner) : Updatable(owner), bombs_(2), bullets_(5)
{

}

void Tank::Update()
{
}

void Tank::LoadFromJson(const json& data)
{
}

json Tank::SaveToJson()
{
	return json();
}

void Tank::OnCollision(Collision* collision)
{
}

void Tank::ShowDetails()
{
}

Component* Tank::CreateComponent(GameObject* owner)
{
	return nullptr;
}
