#pragma once
#include <string>
#include <map>
#include <typeindex>	// std::type_index

class SerializationManager
{
private:
	std::map<std::string, std::type_index> nameToType_;

	SerializationManager() = default;
	~SerializationManager() = default;

	SerializationManager(const SerializationManager&) = delete;
	SerializationManager& operator =(const SerializationManager&) = delete;
	SerializationManager(SerializationManager&&) = delete;
	SerializationManager& operator =(SerializationManager&&) = delete;

public:
	static SerializationManager& GetInstance()
	{
		static SerializationManager instance;
		return instance;
	}

	void LoadState(const std::string& filename);
	void SaveState(const std::string& filename);
};