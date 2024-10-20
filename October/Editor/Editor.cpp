#include "Editor.h"

#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "../Utils/imgui/imgui.h"
#include "../Utils/imgui/imgui_impl_glfw.h"
#include "../Utils/imgui/imgui_impl_opengl3.h"
#include "../Manager/SerializationManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/GameStateManager.h"
#include "../GameObject/GameObject.h"
#include "../State/EmptyState.h"

#include "../Component/FixedUpdatable/Transform.h"
#include "../Component/LateUpdatable/Sprite.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
    extern SerializationManager& serMgr;
    extern GameStateManager& gsMgr;
}

Editor::Editor()
{
#ifdef _DEBUG
    exit = false;
#endif
}

void Editor::Init(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void Editor::ShowEditor()
{
    if (exit)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Topbar();

    if (showObjectList)
        ObjectList();

    if (showObjectDetails)
        ObjectDetails();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Exit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Editor::Topbar()
{
    static std::string path = "Assets/States/";
    static char str[128] = "";
    static bool saveNewState = false;
    static std::string stateFileName = "";

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New State"))
        {
            selectedGameObject = nullptr;
            EmptyState* newState = new EmptyState();
            Manager::gsMgr.ChangeState(newState);
        }

        if (ImGui::BeginMenu("Open State"))
        {
            if (ImGui::BeginCombo("##0 open state", stateFileName.c_str()))
            {
                for (const auto& state : std::filesystem::directory_iterator(path))
                {
                    if (ImGui::MenuItem(state.path().filename().string().c_str()))
                    {
                        selectedGameObject = nullptr;
                        EmptyState* newState = new EmptyState();
                        Manager::gsMgr.ChangeState(newState);

                        stateFileName = state.path().filename().string();
                        Manager::serMgr.LoadState(path + stateFileName);
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Save"/*, "Ctrl+S"*/))
        {
            if (!stateFileName.empty())
                Manager::serMgr.SaveState(path + stateFileName);
            else
                saveNewState = true;
        }
        if (ImGui::MenuItem("Save As..."))
            saveNewState = true;

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Create Empty"))
            Manager::objMgr.CreateObject();

        if (ImGui::MenuItem("Delete", NULL, false, selectedGameObject != nullptr))
        {
            Manager::objMgr.DeleteObject(selectedGameObject);
            selectedGameObject = nullptr;
        }

        if (ImGui::BeginMenu("Rename", selectedGameObject != nullptr))
        {
            if (ImGui::InputTextWithHint("##2 rename", "Enter new name", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Manager::objMgr.RenameObject(selectedGameObject, std::string(str));
                str[0] = '\0';
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Component"))
    {
        if (ImGui::BeginMenu("Add..."))
        {
            if (ImGui::MenuItem("Sprite", NULL, false, selectedGameObject != nullptr && !selectedGameObject->HasComponent(typeid(Sprite))))
                selectedGameObject->AddComponent(typeid(Sprite));

            ImGui::EndMenu();
        }
       
        if (ImGui::BeginMenu("Delete...", selectedGameObject != nullptr))
        {
            if (ImGui::MenuItem("Sprite", NULL, false, selectedGameObject->HasComponent(typeid(Sprite))))
                selectedGameObject->DeleteComponent(typeid(Sprite));

            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        ImGui::MenuItem("GameObject List", NULL, &showObjectList);
        ImGui::MenuItem("GameObject Details", NULL, &showObjectDetails);
        ImGui::EndMenu();
    }

    if (saveNewState)
        ImGui::OpenPopup("Save State");

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Save State", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::InputTextWithHint("##1 save state", ".State", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            stateFileName = path + std::string(str) + ".State";
            Manager::serMgr.SaveState(stateFileName);

            saveNewState = false;
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            saveNewState = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::EndMainMenuBar();
}

void Editor::ObjectList()
{
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImVec2 pos, size;

    pos = size = viewportSize;
    size.x *= 0.14;
    size.y *= 0.2;
    pos.x = size.x / 2;
    pos.y = size.y / 2 + 20;

    ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);

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
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImVec2 pos, size;

    pos = size = viewportSize;
    size.x *= 0.14;
    size.y *= 0.6;
    pos.x = size.x / 2;
    pos.y = viewportSize.y * 0.2 + size.y / 2 + 20;

    ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);

    ImGui::Begin("GameObject Details");

    if (selectedGameObject)
    {
        DetailTransform();

        for (const auto& compIt : selectedGameObject->GetAllComponents())
        {
            if (compIt.first == typeid(Sprite))
                DetailSprite();
        }
    }

    ImGui::End();
}

void Editor::DetailTransform()
{
    Transform* trans = static_cast<Transform*>(selectedGameObject->GetComponent(typeid(Transform)));

    ImGui::SeparatorText("Transform");
    ImGui::Text("Position");
    ImGui::InputFloat2("##Position", trans->GetPosition());

    ImGui::Text("Rotation");
    ImGui::SliderFloat("##Rotation", trans->GetRotation(), -360, 360);

    ImGui::Text("Scale");
    ImGui::InputFloat2("##Scale", trans->GetScale());

    ImGui::Text("Local Scale");
    ImGui::InputFloat2("##Local Scale", trans->GetLocalScale());
}

void Editor::DetailSprite()
{
    Sprite* sp = static_cast<Sprite*>(selectedGameObject->GetComponent(typeid(Sprite)));

    ImGui::SeparatorText("Sprite");
    ImGui::Text("Local Position");
    ImGui::InputFloat2("##local position", sp->GetLocalPosition());

    ImGui::Text("Vertex Colors");
    ImGui::ColorEdit3("Up Right", sp->GetColor(0));
    ImGui::ColorEdit3("Down Right", sp->GetColor(1));
    ImGui::ColorEdit3("Down Left", sp->GetColor(2));
    ImGui::ColorEdit3("Up Left", sp->GetColor(3));
    ImGui::SliderFloat("Alpha", sp->GetAlpha(), 0, 1);
    
    ImGui::Text("Texture");
    std::string test = sp->GetTextureName();
    if (ImGui::BeginCombo("##texture", sp->GetTextureName().c_str()))
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
