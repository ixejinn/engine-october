#include "../Manager/ResourceManager.h"
#include "../Manager/ComponentManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/GameStateManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SerializationManager.h"
#include "../Manager/SettingManager.h"

#include "../Editor/Editor.h"

#include "../Profiler/Profiler.h"

namespace Manager
{
	ResourceManager& rscMgr = ResourceManager::GetInstance();

	ComponentManager& compMgr = ComponentManager::GetInstance();
	GameObjectManager& objMgr = GameObjectManager::GetInstance();

	GameStateManager& gsMgr = GameStateManager::GetInstance();

	//EventManager& evntMgr = EventManager::GetInstance();
	CollisionManager& colMgr = CollisionManager::GetInstance();

	SerializationManager& serMgr = SerializationManager::GetInstance();

	SettingManager& setMgr = SettingManager::GetInstance();
	Editor& editor = Editor::GetInstance();

	OctProfiler::Profiler& profiler = OctProfiler::Profiler::GetInstance();
}