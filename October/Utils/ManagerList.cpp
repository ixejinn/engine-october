#include "../Manager/GameStateManager.h"
#include "../Manager/ComponentManager.h"
#include "../Manager/GameObjectManager.h"

namespace Manager
{
	//ResourceManager& rscMgr = ResourceManager::GetInstance();

	ComponentManager& compMgr = ComponentManager::GetInstance();
	GameObjectManager& objMgr = GameObjectManager::GetInstance();

	GameStateManager& gsMgr = GameStateManager::GetInstance();
	//EventManager& evntMgr = EventManager::GetInstance();
}