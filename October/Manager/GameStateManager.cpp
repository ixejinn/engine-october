#include "GameStateManager.h"

#include <iostream>
#include "../State/State.h"
#include "../Manager/ComponentManager.h"

namespace Manager
{
	extern ComponentManager& compMgr;
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
	// Clear managers

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

bool GameStateManager::ShouldExit()
{
	return curState_ == nullptr;
}
