#include <windows.h>
#include <iostream>
#include <crtdbg.h>	// Check memory leaks

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Manager/GameStateManager.h"
#include "Manager/SettingManager.h"
#include "Editor/Editor.h"

#include "State/EmptyState.h"

namespace Manager
{
	extern GameStateManager& gsMgr;
	extern SettingManager& setMgr;
	extern Editor& editor;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

int main()
{
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(Manager::setMgr.WINDOW_WIDTH, Manager::setMgr.WINDOW_HEIGHT, "October", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	SettingManager::GetInstance().SetWindow(window);
	Manager::editor.Init(window);

	// Set state
	EmptyState* sample = new EmptyState();
	Manager::gsMgr.ChangeState(sample);

	while (!glfwWindowShouldClose(window)/* && !Manager::gsMgr.ShouldExit()*/)
	{
		ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Manager::gsMgr.Update();

		glfwPollEvents();	// Check event, Update window state, Call callback function

		Manager::editor.ShowEditor();

		glfwSwapBuffers(window);
	}
	// !!! 필요없어지면 지우기 !!!
	Manager::gsMgr.ChangeState(nullptr);
	Manager::gsMgr.Exit();

	Manager::editor.Exit();

	glfwTerminate();
	return 0;
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}