#include "GameStateManager.h"

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
	if (curState_ != preState_)
	{
		if (preState_)
		{
			preState_->Exit();
			delete preState_;
		}

		Init();
		preState_ = curState_;
	}

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
	preState_ = curState_;
	curState_ = newState;
}

bool GameStateManager::ShouldExit()
{
	return curState_ == nullptr;
}
