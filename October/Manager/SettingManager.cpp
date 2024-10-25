#include "SettingManager.h"

SettingManager::SettingManager()
{
	window_ = nullptr;
}

void SettingManager::SetWindow(GLFWwindow* window)
{
	window_ = window;
}
