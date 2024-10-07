#pragma once
#include <string>
#include "State.h"

class Sample : public State
{
private:

public:
	void Init() override;
	void Update() override;
	void Exit() override;
};