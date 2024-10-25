#pragma once
#include <GLFW/glfw3.h>

class SettingManager
{
private:
	GLFWwindow* window_;

	SettingManager();
	~SettingManager() = default;

	SettingManager(const SettingManager&) = delete;
	SettingManager& operator =(const SettingManager&) = delete;
	SettingManager(SettingManager&&) = delete;
	SettingManager& operator =(SettingManager&&) = delete;

public:
	const unsigned int WINDOW_WIDTH = 1600;
	const unsigned int WINDOW_HEIGHT = 900;

	static SettingManager& GetInstance()
	{
		static SettingManager instance;
		return instance;
	}

	GLFWwindow* GetWindow() { return window_; }

	void SetWindow(GLFWwindow* window);
};