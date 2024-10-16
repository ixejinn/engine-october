#pragma once
#include <string>

class Resource
{
private:
	int counter_ = 0;
	bool persistent_ = false;

protected:
	void* data_ = nullptr;

public:
	virtual ~Resource() = default;

	virtual void Load(const std::string& name) = 0;
	virtual void Unload() = 0;

	friend class ResourceManager;
};