#include "GameStateManager.h"

#include <iostream>
#include "../State/State.h"
#include "../Manager/ComponentManager.h"
#include "../Manager/GameObjectManager.h"

namespace Manager
{
	extern ComponentManager& compMgr;
	extern GameObjectManager& objMgr;
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
		curState_->Update();

		// Update components
		Manager::compMgr.UpdateComponent();
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
}

bool GameStateManager::ShouldExit()
{
	return curState_ == nullptr;
}
