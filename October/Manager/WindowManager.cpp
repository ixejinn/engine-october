#include "WindowManager.h"

GLFWwindow* WindowManager::window_ = nullptr;

void WindowManager::SetWindow(GLFWwindow* window)
{
	window_ = window;
}
