#include "Editor.h"

#include <filesystem>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "../Utils/imgui/imgui.h"
#include "../Utils/imgui/imgui_impl_glfw.h"
#include "../Utils/imgui/imgui_impl_opengl3.h"
#include "../Utils/imgui/implot.h"
#include "../Utils/imgui/implot_internal.h"
#include "../Manager/SerializationManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/GameStateManager.h"
#include "../Profiler/Profiler.h"
#include "../GameObject/GameObject.h"
#include "../State/EmptyState.h"

#include "../Component/Transform.h"
#include "../Component/Rigidbody.h"
#include "../Component/FixedUpdatable/BoxCollider.h"
#include "../Component/FixedUpdatable/CircleCollider.h"
#include "../Component/Updatable/PlayerController.h"
#include "../Component/Updatable/Gameplay/Player.h"
#include "../Component/LateUpdatable/Sprite.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
    extern SerializationManager& serMgr;
    extern GameStateManager& gsMgr;
    extern OctProfiler::Profiler& profiler;
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
        Manager::serMgr.LoadState("Assets/States/.temp.state");
    }
    else
    {
        selectedGameObject_ = nullptr;
        Manager::serMgr.SaveState("Assets/States/.temp.state");
    }
}

void Editor::Init(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void Editor::ShowEditor()
{
    DEBUG_PROFILER_BLOCK_START(__FUNCTION_NAME__);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImPlot::ShowDemoWindow();
    EditmodeButton();
    
    if (mode_)
    {
        Topbar();

        if (showObjectList_)
            ObjectList();

        if (showObjectDetails_)
            ObjectDetails();
    }

    if (Manager::profiler.active_)
        ProfilerGraph();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    DEBUG_PROFILER_BLOCK_END;
}

void Editor::Exit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void Editor::Topbar()
{
    static const std::string path = "Assets/States/";
    static char str[128] = "";
    static bool saveNewState = false;
    static std::string curStateName = "";

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New State"))
        {
            selectedGameObject_ = nullptr;
            curStateName.clear();

            EmptyState* newState = new EmptyState();
            Manager::gsMgr.ChangeState(newState);
        }

        if (ImGui::BeginMenu("Open State"))
        {
            if (ImGui::BeginCombo("##open state", curStateName.c_str()))
            {
                for (const auto& state : std::filesystem::directory_iterator(path))
                {
                    std::string filename = state.path().filename().string();
                    if (filename[0] == '.')
                        continue;

                    if (ImGui::MenuItem(filename.c_str()))
                    {
                        selectedGameObject_ = nullptr;
                        EmptyState* newState = new EmptyState();
                        Manager::gsMgr.ChangeState(newState);

                        curStateName = filename;
                        Manager::serMgr.LoadState(path + curStateName);
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Save"/*, "Ctrl+S"*/))
        {
            if (!curStateName.empty())
                Manager::serMgr.SaveState(path + curStateName);
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
        bool selected = (selectedGameObject_ != nullptr);
        bool hasRb, hasBoxCol, hasCirCol;
        bool hasPc, hasPlayer;
        bool hasSp;
        hasRb = hasBoxCol = hasCirCol = false;
        hasPc = hasPlayer = false;
        hasSp = false;

        if (selected)
        {
            hasRb = selectedGameObject_->HasComponent(typeid(Rigidbody));
            hasBoxCol = selectedGameObject_->HasComponent(typeid(BoxCollider));
            hasCirCol = selectedGameObject_->HasComponent(typeid(CircleCollider));

            hasPc = selectedGameObject_->HasComponent(typeid(PlayerController));
            hasPlayer = selectedGameObject_->HasComponent(typeid(Player));

            hasSp = selectedGameObject_->HasComponent(typeid(Sprite));
        }

        if (ImGui::BeginMenu("Add...", selected))
        {
            if (ImGui::MenuItem("Rigidbody", NULL, false, !hasRb))
                selectedGameObject_->AddComponent(typeid(Rigidbody));

            if (ImGui::BeginMenu("Collider", hasRb))
            {
                if (ImGui::MenuItem("Box Collider", NULL, false, !hasBoxCol))
                    selectedGameObject_->AddComponent(typeid(BoxCollider));

                if (ImGui::MenuItem("Circle Collider", NULL, false, !hasCirCol))
                    selectedGameObject_->AddComponent(typeid(CircleCollider));

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Player Controller", NULL, false, hasRb && !hasPc))
                selectedGameObject_->AddComponent(typeid(PlayerController));

            if (ImGui::MenuItem("Player", NULL, false, hasRb && hasSp && !hasPlayer))
                selectedGameObject_->AddComponent(typeid(Player));

            if (ImGui::MenuItem("Sprite", NULL, false, !hasSp))
                selectedGameObject_->AddComponent(typeid(Sprite));

            ImGui::EndMenu();
        }
       
        if (ImGui::BeginMenu("Delete...", selected))
        {
            if (ImGui::MenuItem("Rigidbody", NULL, false, hasRb && !hasBoxCol && !hasCirCol && !hasPc && !hasPlayer))
            {
                selectedGameObject_->DeleteComponent(typeid(Rigidbody));

                selectedGameObject_->DeleteComponent(typeid(Rigidbody));
                selectedGameObject_->DeleteComponent(typeid(PlayerController));
            }

            if (ImGui::BeginMenu("Collider", hasRb))
            {
                if (ImGui::MenuItem("Box Collider", NULL, false, hasBoxCol))
                    selectedGameObject_->DeleteComponent(typeid(BoxCollider));

                if (ImGui::MenuItem("Circle Collider", NULL, false, hasCirCol))
                    selectedGameObject_->DeleteComponent(typeid(CircleCollider));

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Player Controller", NULL, false, hasPc))
                selectedGameObject_->DeleteComponent(typeid(PlayerController));

            if (ImGui::MenuItem("Player", NULL, false, hasPlayer))
                selectedGameObject_->DeleteComponent(typeid(Player));

            if (ImGui::MenuItem("Sprite", NULL, false, hasSp && !hasPlayer))
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
        if (ImGui::InputTextWithHint("##save state", "filename(.state)", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            curStateName = path + std::string(str) + ".state";
            Manager::serMgr.SaveState(curStateName);

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
    size.x *= 0.18f;
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
    size.x *= 0.18f;
    size.y *= 0.6f;
    pos.x = size.x / 2;
    pos.y = viewportSize.y * 0.2f + size.y / 2.f + 20;

    ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);

    ImGui::Begin("GameObject Details");

    if (selectedGameObject_)
    {
        selectedGameObject_->GetComponent(typeid(Transform))->ShowDetails();

        for (const auto& compIt : selectedGameObject_->GetAllComponents())
        {
            if (compIt.first != typeid(Transform))
                compIt.second->ShowDetails();
        }
    }

    ImGui::End();
}

void Editor::ProfilerGraph()
{
    float t = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - Manager::profiler.rootStart_).count());
    static float history = Manager::profiler.GetScrollingBufferMaxSize() / 60.f * 1000;
    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

    ImGui::SetNextWindowSize(ImVec2(1000, 180), ImGuiCond_Appearing);

    ImGui::Begin("Execution Timeline", 0, ImGuiWindowFlags_NoScrollbar);

    if (ImPlot::BeginPlot("##ProfilerGraph", ImVec2(-1, 130)))
    {
        ImPlot::SetupAxes(nullptr, "micro seconds", flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 20000);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

        for (const auto& pair : Manager::profiler.graphData_)
        {
            ImPlot::PlotLine(pair.first.c_str(), &pair.second->data_[0].x, &pair.second->data_[0].y, pair.second->data_.size(), 0, pair.second->offset_, 2 * sizeof(float));
        }

        ImPlot::EndPlot();
    }
    
    ImGui::Text("%.3f ms/frame (60 FPS = 16.666 ms)", Manager::profiler.mainExecutionTime);
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