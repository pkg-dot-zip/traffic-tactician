#include "KeyboardInputHandler.h"

#include <ostream>

#include "easylogging++.h"
#include "InputHandler.h"

void initKeyCallback(GLFWwindow* window)
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(window, true);
			}

			if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
			{
				setInputPose(POSE_MOVE_FORWARD);
			}

			if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
			{
				setInputPose(POSE_MOVE_LEFT);
			}

			if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
			{
				setInputPose(POSE_MOVE_RIGHT);
			}

			if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
			{
				setInputPose(POSE_STOP);
			}

			if (key == GLFW_KEY_5 && action == GLFW_RELEASE)
			{
				setInputPose(POSE_OTHER);
			}
		});


	LOG(INFO) << "Initialized input callback." << std::endl;
}
