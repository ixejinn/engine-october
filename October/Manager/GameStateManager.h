#pragma once

class State;

class GameStateManager
{
private:
	GameStateManager() = default;
	~GameStateManager() = default;

	GameStateManager(const GameStateManager&) = delete;
	GameStateManager& operator =(const GameStateManager&) = delete;
	GameStateManager(GameStateManager&&) = delete;
	GameStateManager& operator =(GameStateManager&&) = delete;

	State* preState_ = nullptr;
	State* curState_ = nullptr;

public:
	static GameStateManager& GetInstance()
	{
		static GameStateManager instance;
		return instance;
	}

	void Init();
	void Update();
	void Exit();

	void ChangeState(State* newState);

	bool ShouldExit();
};