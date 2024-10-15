#include "Editor.h"

#include <glm/vec2.hpp>
#include "../Utils/imgui/imgui.h"
#include "../Manager/SerializationManager.h"
#include "../Manager/GameObjectManager.h"
#include "../GameObject/GameObject.h"

#include "../Component/FixedUpdatable/Transform.h"
#include "../Component/LateUpdatable/Sprite.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
    extern SerializationManager& serMgr;
}

void Editor::ShowEditor()
{
    Topbar();

    if (showObjectList)
        ObjectList();

    if (showObjectDetails)
        ObjectDetails();
}

void Editor::Topbar()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        //if (ImGui::MenuItem("New State")) {}
        //if (ImGui::MenuItem("Open State")) {}

        //ImGui::Separator();
        if (ImGui::MenuItem("Save"/*, "Ctrl+S"*/))
        {
            Manager::serMgr.SaveState("Assets/States/editorTest.state");
        }
        if (ImGui::MenuItem("Save As...")) {}

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Create Empty"))
        {
            Manager::objMgr.CreateObject();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Component"))
    {
        if (ImGui::BeginMenu("Add..."))
        {
            if (ImGui::MenuItem("Sprite", NULL, false, selectedGameObject != nullptr))
            {
                selectedGameObject->AddComponent(typeid(Sprite));
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        ImGui::MenuItem("GameObject List", NULL, &showObjectList);
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void Editor::ObjectList()
{
    ImGui::Begin("GameObject List");

    for (const auto& objIt : Manager::objMgr.GetAllObjects())
    {
        GameObject* obj = objIt.second.get();
        if (ImGui::Selectable(objIt.second->GetName().c_str(), selectedGameObject == obj))
        {
            if (selectedGameObject == obj)
                selectedGameObject = nullptr;
            else
                selectedGameObject = obj;
        }
    }

    ImGui::End();
}

void Editor::ObjectDetails()
{
    ImGui::Begin("GameObject Details");

    if (selectedGameObject)
    {
        for (const auto& compIt : selectedGameObject->GetAllComponents())
        {
            if (compIt.first == typeid(Transform))
                DetailTransform();
            else if (compIt.first == typeid(Sprite))
                DetailSprite();
        }
    }

    ImGui::End();
}

void Editor::DetailTransform()
{
    Transform* trans = static_cast<Transform*>(selectedGameObject->GetComponent(typeid(Transform)));

    ImGui::SeparatorText("Transform");
    ImGui::InputFloat2("Position", trans->GetPosition());
    ImGui::SliderFloat("Rotation", trans->GetRotation(), -360, 360);
    ImGui::InputFloat2("Scale", trans->GetScale());
    ImGui::InputFloat2("Local Scale", trans->GetLocalScale());
}

void Editor::DetailSprite()
{
    Sprite* sp = static_cast<Sprite*>(selectedGameObject->GetComponent(typeid(Sprite)));

    ImGui::SeparatorText("Sprite");

}
