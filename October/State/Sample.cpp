#include "Sample.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Manager/GameObjectManager.h"
#include "../Manager/SerializationManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/LateUpdatable/Sprite.h"
#include "../Component/FixedUpdatable/Transform.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
    extern SerializationManager& serMgr;
}

void Sample::Init()
{
    //GameObject* obj = Manager::objMgr.CreateObject();
    //obj->AddComponent(typeid(Sprite));

    //sp = static_cast<Sprite*>(obj->GetComponent(typeid(Sprite)));
    ////sp->SetTexture("Assets/IMG_4166.JPEG");

    //Transform* trans = static_cast<Transform*>(obj->GetComponent(typeid(Transform)));
    //trans->SetPosition(glm::vec2(5, 0));
    //trans->SetScale(glm::vec2(1.f, 1.f));
    //trans->SetRotation(-25);

    //GameObject* other = Manager::objMgr.CreateObject();
    //other->AddComponent(typeid(Sprite));

    //static_cast<Sprite*>(other->GetComponent(typeid(Sprite)))->SetTexture("Assets/PlanetTexture.png");
    //trans = static_cast<Transform*>(other->GetComponent(typeid(Transform)));
    //trans->SetPosition(glm::vec2(-5, 0));
    //trans->SetScale(glm::vec2(5.f, 5.f));

    //Manager::serMgr.SaveState("Sample.state");


    Manager::serMgr.LoadState("Sample.state");
}

void Sample::Update()
{
    //double timeValue = glfwGetTime();
    //float colorValue = float(sin(timeValue)) / 2.0f + 0.5f;
    //sp->SetColor(0, { colorValue, colorValue, colorValue });
    //sp->SetColor(1, { colorValue, 0, 0 });
    //sp->SetColor(2, { 0, colorValue, 0 });
    //sp->SetColor(3, { 0, 0, colorValue });
}

void Sample::Exit()
{
}