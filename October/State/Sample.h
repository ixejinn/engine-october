#pragma once
#include <string>
#include "State.h"

class Sprite;

class Sample : public State
{
private:
	Sprite* sp;

public:
	void Init() override;
	void Update() override;
	void Exit() override;
};