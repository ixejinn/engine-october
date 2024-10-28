#include "GameStateManager.h"

#include <iostream>
#include "../State/State.h"
#include "../Manager/ComponentManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/CollisionManager.h"
#include "../Editor/Editor.h"

namespace Manager
{
	extern ComponentManager& compMgr;
	extern GameObjectManager& objMgr;
	extern CollisionManager& colMgr;
	extern Editor& editor;
}

void GameStateManager::Init()
{
	if (curState_)
		curState_->Init();
}

void GameStateManager::Update()
{
	if (curState_)
	{
		// Physics
		Manager::compMgr.FixedUpdate();
		if (!Manager::editor.GetMode())
			Manager::colMgr.CheckAllCollisions();

		// Game logic
		curState_->Update();
		Manager::compMgr.Update();

		// Rendering
		Manager::compMgr.LateUpdate();
		
	}
}

void GameStateManager::Exit()
{
	ClearManagers();

	preState_->Exit();
	delete preState_;
}

void GameStateManager::ChangeState(State* newState)
{
	if (curState_)
		curState_->Exit();

	preState_ = curState_;
	curState_ = newState;

	if (curState_)
		curState_->Init();
}

void GameStateManager::ClearManagers()
{
	Manager::objMgr.Clear();
	Manager::compMgr.Clear();
	Manager::colMgr.Clear();
}

bool GameStateManager::ShouldExit()
{
	return curState_ == nullptr;
}
