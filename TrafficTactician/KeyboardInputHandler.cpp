#include "KeyboardInputHandler.h"

#include <ostream>

#include "easylogging++.h"

void initKeyCallback(GLFWwindow* window)
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(window, true);
			}
		});


	LOG(INFO) << "Initialized input callback." << std::endl;
}
