#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class GameObject;

class Editor
{
private:
	bool mode_{ false };

	bool showObjectList_{ true };
	bool showObjectDetails_{ true };

	GameObject* selectedGameObject_{ nullptr };

	Editor();
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

	bool GetMode() const { return mode_; }

	void ToggleMode();

	void Init(GLFWwindow* window);
	void ShowEditor();
	void Exit();

	void Topbar();
	void ObjectList();
	void ObjectDetails();
	void EditmodeButton();
};