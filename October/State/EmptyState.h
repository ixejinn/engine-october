#pragma once
#include "State.h"

class EmptyState : public State
{
public:
	void Init() override;
	void Update() override;
	void Exit() override;
};