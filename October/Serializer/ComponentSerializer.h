#pragma once
#include "json/json.hpp"

using json = nlohmann::ordered_json;
//using json = nlohmann::json;

class ComponentSerializer
{
public:
	virtual ~ComponentSerializer() = default;

	virtual void LoadFromJson(const json&) = 0;
	virtual json SaveToJson() = 0;
};