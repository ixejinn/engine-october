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
    mode_ = true;
#endif
}

void Editor::ToggleMode()
{
    mode_ = !mode_;

    if (mode_ == true)
    {
        Manager::gsMgr.ClearManagers();
        Manager::gsMgr.Init();
    }
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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    EditmodeButton();
    
    if (mode_)
    {
        Topbar();

        if (showObjectList_)
            ObjectList();

        if (showObjectDetails_)
            ObjectDetails();
    }

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
    static const std::string path = "Assets/States/";
    static char str[128] = "";
    static bool saveNewState = false;
    static std::string stateFileName = "";

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New State"))
        {
            selectedGameObject_ = nullptr;
            EmptyState* newState = new EmptyState();
            Manager::gsMgr.ChangeState(newState);
        }

        if (ImGui::BeginMenu("Open State"))
        {
            if (ImGui::BeginCombo("##open state", stateFileName.c_str()))
            {
                for (const auto& state : std::filesystem::directory_iterator(path))
                {
                    if (ImGui::MenuItem(state.path().filename().string().c_str()))
                    {
                        selectedGameObject_ = nullptr;
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

        if (ImGui::MenuItem("Delete", NULL, false, selectedGameObject_ != nullptr))
        {
            Manager::objMgr.DeleteObject(selectedGameObject_);
            selectedGameObject_ = nullptr;
        }

        if (ImGui::BeginMenu("Rename", selectedGameObject_ != nullptr))
        {
            if (ImGui::InputTextWithHint("##rename", "Enter new name", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Manager::objMgr.RenameObject(selectedGameObject_, std::string(str));
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
            if (ImGui::MenuItem("Sprite", NULL, false, selectedGameObject_ != nullptr && !selectedGameObject_->HasComponent(typeid(Sprite))))
                selectedGameObject_->AddComponent(typeid(Sprite));

            ImGui::EndMenu();
        }
       
        if (ImGui::BeginMenu("Delete...", selectedGameObject_ != nullptr))
        {
            if (ImGui::MenuItem("Sprite", NULL, false, selectedGameObject_->HasComponent(typeid(Sprite))))
                selectedGameObject_->DeleteComponent(typeid(Sprite));

            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        ImGui::MenuItem("GameObject List", NULL, &showObjectList_);
        ImGui::MenuItem("GameObject Details", NULL, &showObjectDetails_);
        ImGui::EndMenu();
    }

    if (saveNewState)
        ImGui::OpenPopup("Save State");

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Save State", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::InputTextWithHint("##save state", ".State", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue))
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

    size = viewportSize;
    size.x *= 0.14f;
    size.y *= 0.2f;
    pos.x = size.x / 2.f;
    pos.y = size.y / 2.f + 20;

    ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);

    ImGui::Begin("GameObject List");

    for (const auto& objIt : Manager::objMgr.GetAllObjects())
    {
        GameObject* obj = objIt.second.get();
        if (ImGui::Selectable(objIt.second->GetName().c_str(), selectedGameObject_ == obj))
        {
            if (selectedGameObject_ == obj)
                selectedGameObject_ = nullptr;
            else
                selectedGameObject_ = obj;
        }
    }

    ImGui::End();
}

void Editor::ObjectDetails()
{
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImVec2 pos, size;

    size = viewportSize;
    size.x *= 0.14f;
    size.y *= 0.6f;
    pos.x = size.x / 2;
    pos.y = viewportSize.y * 0.2f + size.y / 2.f + 20;

    ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);

    ImGui::Begin("GameObject Details");

    if (selectedGameObject_)
    {
        DetailTransform();

        for (const auto& compIt : selectedGameObject_->GetAllComponents())
        {
            if (compIt.first == typeid(Sprite))
                DetailSprite();
        }
    }

    ImGui::End();
}

void Editor::EditmodeButton()
{
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImVec2 pos, size;

    pos = viewportSize;
    pos.x += 15;
    pos.y = 15;
    size.x = 80;
    size.y = 80;

    ImGui::SetNextWindowPos(pos, ImGuiCond_Once, ImVec2(1.f, 0.f));
    ImGui::SetNextWindowSize(size);

    ImGui::Begin("##edit mode", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

    if (ImGui::Button((mode_ ? "Play" : "Edit"), ImVec2(size.x - 30, size.y - 30)))
        ToggleMode();

    ImGui::End();
}

void Editor::DetailTransform()
{
    Transform* trans = static_cast<Transform*>(selectedGameObject_->GetComponent(typeid(Transform)));

    ImGui::SeparatorText("Transform");
    ImGui::Text("Position");
    ImGui::InputFloat2("##position", trans->GetPosition());

    ImGui::Text("Rotation");
    ImGui::SliderFloat("##rotation", trans->GetRotation(), -360, 360);

    ImGui::Text("Scale");
    ImGui::InputFloat2("##scale", trans->GetScale());

    ImGui::Text("Local Scale");
    ImGui::InputFloat2("##local scale", trans->GetLocalScale());
}

void Editor::DetailSprite()
{
    Sprite* sp = static_cast<Sprite*>(selectedGameObject_->GetComponent(typeid(Sprite)));

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
