#pragma once

class State
{
public:
	virtual ~State() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
};