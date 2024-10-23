#pragma once
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <type_traits>	// std::is_same

#include "../Resource/Resource.h"
#include "../Resource/Shader.h"
#include "../Resource/Texture.h"

class ResourceManager
{
private:
	enum Extension
	{
		SHADER,
		PNG,
		JPEG
	};
	std::map<std::string, Extension> stringToExtension_
	{
		{ "fs", SHADER },
		{ "png", PNG },
		{ "jpeg", JPEG }
	};

	std::map<Extension, std::map<std::string, std::unique_ptr<Resource>>> resources_{};

	ResourceManager() = default;
	~ResourceManager();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator =(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator =(ResourceManager&&) = delete;

	Extension StringToExtension(const std::string& filename);

public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	template <typename T>
	T* Load(const std::string& filename);

	void Unload(const std::string& filename);
	void UnloadAll(bool unloadAll);
};


template<typename T>
inline T* ResourceManager::Load(const std::string& filename)
{
	Extension ext = StringToExtension(filename);

	if (resources_[ext].find(filename) == resources_[ext].end())
	{
		std::unique_ptr<T> ptr(new T());
		resources_[ext].insert({ filename, std::move(ptr) });
		resources_[ext][filename]->Load(filename);
	}

	Resource* resource = resources_[ext][filename].get();
	resource->counter_++;

	return static_cast<T*>(resource);
}
