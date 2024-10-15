#include "../Manager/GameStateManager.h"
#include "../Manager/ComponentManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/SerializationManager.h"

#include "../Editor/Editor.h"

namespace Manager
{
	//ResourceManager& rscMgr = ResourceManager::GetInstance();

	GameObjectManager& objMgr = GameObjectManager::GetInstance();
	ComponentManager& compMgr = ComponentManager::GetInstance();

	GameStateManager& gsMgr = GameStateManager::GetInstance();
	//EventManager& evntMgr = EventManager::GetInstance();

	SerializationManager& serMgr = SerializationManager::GetInstance();

	Editor& editor = Editor::GetInstance();
}