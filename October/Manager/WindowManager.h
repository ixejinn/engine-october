#pragma once
#include <GLFW/glfw3.h>

class WindowManager
{
private:
	static GLFWwindow* window_;

public:
	static GLFWwindow* GetWindow() { return window_; }
	static void SetWindow(GLFWwindow* window);
};