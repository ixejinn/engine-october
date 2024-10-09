#include "Sample.h"

#include "../Manager/GameObjectManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/LateUpdatable/Sprite.h"
#include "../Component/FixedUpdatable/Transform.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
}

void Sample::Init()
{
    GameObject* obj = Manager::objMgr.CreateObject();
    obj->AddComponent(typeid(Sprite));

    Transform* trans = static_cast<Transform*>(obj->GetComponent(typeid(Transform)));
    trans->SetPosition(glm::vec2(5, 0));
    trans->SetScale(glm::vec2(1.f, 1.f));
    trans->SetRotation(-25);
}

void Sample::Update()
{
}

void Sample::Exit()
{
}