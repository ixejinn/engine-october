#include "Sample.h"

#include "../Manager/GameObjectManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/FixedUpdatable/Transform.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
}


void Sample::Init()
{
    GameObject* obj = Manager::objMgr.CreateObject();
    obj->AddComponent(typeid(Transform));
    obj->DeleteComponent(typeid(Transform));
}

void Sample::Update()
{

}

void Sample::Exit()
{

}