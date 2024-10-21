#pragma once

class State;

class GameStateManager
{
private:
	State* preState_ = nullptr;
	State* curState_ = nullptr;

	GameStateManager() = default;
	~GameStateManager() = default;

	GameStateManager(const GameStateManager&) = delete;
	GameStateManager& operator =(const GameStateManager&) = delete;
	GameStateManager(GameStateManager&&) = delete;
	GameStateManager& operator =(GameStateManager&&) = delete;

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