#include "GameStateManager.h"

#include <iostream>
#include "../State/State.h"
#include "../Manager/ComponentManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/CollisionManager.h"
#include "../Editor/Editor.h"
#include "../Profiler/Profiler.h"

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
		PhysicsUpdate();

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
	ClearManagers();

	if (preState_)
		delete preState_;

	preState_ = curState_;
	curState_ = newState;

	if (curState_)
		curState_->Init();
}

void GameStateManager::PhysicsUpdate()
{
	static std::chrono::duration<long, std::milli> adder{ 0 };
	static std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();
	adder += std::chrono::duration_cast<std::chrono::milliseconds>(cur - start);
	if (adder >= Step_)
		start = cur;

	while (adder >= Step_)
	{
		DEBUG_PROFILER_BLOCK_START(__FUNCTION_NAME__);

		Manager::compMgr.FixedUpdate();

		if (!Manager::editor.GetMode())
		{
			Manager::colMgr.CheckAllCollisions();
			Manager::compMgr.RigidbodyUpdate();
		}
		
		Manager::compMgr.TransformUpdate();

		adder -= Step_;

		DEBUG_PROFILER_BLOCK_END;
	}
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
