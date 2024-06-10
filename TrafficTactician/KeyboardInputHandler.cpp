#include "KeyboardInputHandler.h"

#include <ostream>

#include "easylogging++.h"
#include "InputHandler.h"
#include "SoundHandler.h"

bool enableWireframe = false;

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

			if (key == GLFW_KEY_0 && action == GLFW_RELEASE)
			{
				SoundHandler::getInstance().playSoundSnippet("test.wav");
			}

			if (key == GLFW_KEY_9 && action == GLFW_RELEASE)
			{
				SoundHandler::getInstance().forceStopSound();
			}

			if (key == GLFW_KEY_8 && action == GLFW_RELEASE)
			{
				enableWireframe = !enableWireframe;
				glPolygonMode(GL_FRONT_AND_BACK, enableWireframe ? GL_FILL : GL_LINE);
			}
		});


	LOG(INFO) << "Initialized input callback." << std::endl;
}
