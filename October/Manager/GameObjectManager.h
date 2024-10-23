#pragma once
#include <map>
#include <string>
#include <memory>

class GameObject;

class GameObjectManager
{
private:
	std::map<std::string, std::unique_ptr<GameObject>> objects_{};

	int noNameObject_{ 0 };

	GameObjectManager() = default;
	~GameObjectManager() = default;

	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator =(const GameObjectManager&) = delete;
	GameObjectManager(GameObjectManager&&) = delete;
	GameObjectManager& operator =(GameObjectManager&&) = delete;

public:
	static GameObjectManager& GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}

	const std::map<std::string, std::unique_ptr<GameObject>>& GetAllObjects() const { return objects_; }
	GameObject* GetObject(const std::string& name) { return objects_[name].get(); }

	GameObject* CreateObject();
	GameObject* CreateObject(const std::string& name);

	void RenameObject(const GameObject* const obj, const std::string& newName);
	void RenameObject(const std::string& name, const std::string& newName);

	void DeleteObject(const GameObject* const obj);
	void DeleteObject(const std::string& name);

	void Clear();
};