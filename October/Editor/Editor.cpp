#include "Editor.h"

#include <filesystem>
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
        static std::string stateFileName = "";

        //if (ImGui::MenuItem("New State")) {}
        if (ImGui::BeginMenu("Open State"))
        {
            if (ImGui::BeginCombo("##open state", NULL))
            {
                std::string path = "Assets/States/";

                for (const auto& state : std::filesystem::directory_iterator(path))
                {
                    if (ImGui::MenuItem(state.path().filename().string().c_str()))
                    {
                        stateFileName = state.path().filename().string();
                        Manager::serMgr.LoadState(stateFileName);
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::EndMenu();
        }

        ImGui::Separator();
        static bool saveNewState = false;

        if (ImGui::MenuItem("Save"/*, "Ctrl+S"*/))
        {
            if (!stateFileName.empty())
                Manager::serMgr.SaveState(stateFileName);
            else
                saveNewState = true;
        }
        if (ImGui::MenuItem("Save As..."))
            saveNewState = true;

        if (saveNewState)
        {
            ImGui::Begin("Save State");

            static char str[128] = "";
            if (ImGui::InputTextWithHint("##", ".State", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                stateFileName = "Assets/States/" + std::string(str) + ".State";
                Manager::serMgr.SaveState(stateFileName);

                saveNewState = false;
            }

            ImGui::End();
        }

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
    ImGui::InputFloat2("Local Position", sp->GetLocalPosition());
    ImGui::Text("Vertex Colors");
    ImGui::ColorEdit3("Up Right", sp->GetColor(0));
    ImGui::ColorEdit3("Down Right", sp->GetColor(1));
    ImGui::ColorEdit3("Down Left", sp->GetColor(2));
    ImGui::ColorEdit3("Up Left", sp->GetColor(3));
    ImGui::SliderFloat("Alpha", sp->GetAlpha(), 0, 1);
    
    if (ImGui::BeginCombo("Texture", sp->GetTextureName().c_str()))
    {
        std::string path = "Assets/Images/";

        for (const auto& txr : std::filesystem::directory_iterator(path))
        {
            if (ImGui::MenuItem(txr.path().filename().string().c_str()))
                sp->SetTexture(path + txr.path().filename().string());
        }

        ImGui::EndCombo();
    }
}
