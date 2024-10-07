#include "Sample.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include "../Manager/GameObjectManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/FixedUpdatable/Transform.h"
#include "../Component/LateUpdatable/Sprite.h"

#include "../Graphics/stb/stb_image.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
}


void Sample::Init()
{
    GameObject* obj = Manager::objMgr.CreateObject();
    obj->AddComponent(typeid(Sprite));
}

void Sample::Update()
{
}

void Sample::Exit()
{
}