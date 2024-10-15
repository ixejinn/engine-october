#pragma once

class GameObject;

class Editor
{
private:
	bool showObjectList = true;
	bool showObjectDetails = true;

	GameObject* selectedGameObject = nullptr;

	Editor() = default;
	~Editor() = default;

	Editor(const Editor&) = delete;
	Editor& operator =(const Editor&) = delete;
	Editor(Editor&&) = delete;
	Editor& operator =(Editor&&) = delete;

public:
	static Editor& GetInstance()
	{
		static Editor instance;
		return instance;
	}

	void ShowEditor();

	void Topbar();
	void ObjectList();
	void ObjectDetails();

	void DetailTransform();
	void DetailSprite();
};