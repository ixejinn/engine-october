#include "ResourceManager.h"

#include <iostream>
#include <algorithm>

ResourceManager::~ResourceManager()
{
	UnloadAll(true);
}

void ResourceManager::Unload(const std::string& filename)
{
	Extension ext = StringToExtension(filename);

	if (resources_[ext].find(filename) == resources_[ext].end())
	{
		std::cerr << "[ERROR] ResourceManager::Unload  Invalid resource " + filename << std::endl;
		return;
	}

	Resource* resource = resources_[ext][filename].get();
	resource->counter_--;
	if (resource->counter_ == 0 && !resource->persistent_)
		resources_[ext].erase(filename);
}

void ResourceManager::UnloadAll(bool unloadAll)
{
	for (auto extIt = resources_.begin(); extIt != resources_.end(); )
	{
		for (auto resIt = extIt->second.begin(); resIt != extIt->second.end(); )
		{
			if (unloadAll || !resIt->second->persistent_)
				extIt->second.erase(resIt++);
			else
				resIt++;
		}

		if (extIt->second.empty())
			resources_.erase(extIt++);
		else
			extIt++;
	}
}

ResourceManager::Extension ResourceManager::StringToExtension(const std::string& filename)
{
	std::string extStr = filename.substr(filename.find_last_of('.') + 1);
	std::transform(extStr.begin(), extStr.end(), extStr.begin(), ::tolower);

	if (stringToExtension_.find(extStr) == stringToExtension_.end())
		std::cerr << "[ERROR] ResourceManager::StringToExtension Invalid file name " + filename << std::endl;
	return stringToExtension_[extStr];
}
