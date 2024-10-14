#include "SerializationManager.h"

#include <fstream>
#include <iostream>
#include <string>
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "../Serializer/ComponentSerializer.h"
#include "../GameObject/GameObject.h"
#include "../Component/Component.h"
#include "../Component/FixedUpdatable/Transform.h"

namespace Manager
{
	extern GameObjectManager& objMgr;
	extern ComponentManager& compMgr;
}

void SerializationManager::LoadState(const std::string& filename)
{
	std::fstream file;
	file.open(filename, std::fstream::in);
	if (!file.is_open())
	{
		std::cerr << "[ERROR] SerializationManager::LoadState Unable to open the file " << filename << std::endl;
		return;
	}

	json stateData;
	file >> stateData;

	for (auto& objData : stateData)
	{
		auto objIt = objData.find("object");
		if (objIt != objData.end())
		{
			GameObject* obj = Manager::objMgr.CreateObject(objIt.value());

			auto compIt = objData.find("components");
			if (compIt == objData.end())
				continue;

			for (auto& compData : *compIt)
			{
				auto compDataIt = compData.find("type");
				if (compDataIt == compData.end())
					continue;
				
				Component* comp = Manager::compMgr.CreateComponent(compDataIt.value(), obj);
				if (comp != nullptr)
					comp->LoadFromJson(compData);
			}
		}
	}
}

void SerializationManager::SaveState(const std::string& filename)
{
	json stateData;

	for (const auto& objIt : Manager::objMgr.GetAllObjects())
	{
		json objData, compData;
		GameObject* obj = objIt.second.get();

		objData["object"] = obj->GetName();

		Component* comp = obj->GetMainComp();
		if (comp)
			compData.push_back(comp->SaveToJson());
		else
		{
			Component* trans = obj->GetComponent(typeid(Transform));
			compData.push_back(trans->SaveToJson());

			for (const auto& compIt : obj->GetAllComponents())
			{
				comp = compIt.second.get();
				if (comp == trans)
					continue;
				
				compData.push_back(comp->SaveToJson());
			}
		}

		objData["components"] = compData;
		stateData.push_back(objData);
	}

	std::fstream file;
	file.open(filename, std::fstream::out);
	if (!file.is_open())
	{
		std::cerr << "[ERROR] SerializationManager::SaveState Unable to open the file " << filename << std::endl;
		return;
	}

	file << std::setw(2) << stateData;
	file.close();
}