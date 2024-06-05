#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MouseDebugComponent.h"
#include "GameObject.h"
#include "Simulation.h"
#include <easylogging++.h>
#include <glm/gtx/string_cast.hpp>


extern GLFWwindow* window;
extern std::vector<tigl::Vertex> verts;

MouseDebugComponent::MouseDebugComponent()
{
}

MouseDebugComponent::~MouseDebugComponent()
{
}

void MouseDebugComponent::update(float deltaTime)
{

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		allowPointClicks = !allowPointClicks;
		LOG(INFO) << "Clicks allowed" << std::endl;
	}

	if (!allowPointClicks) return;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && lastSpawn + clickDelay < glfwGetTime())
	{
		glm::vec3 rayOrigin = gameObject->sim->getCameraPosition();
		glm::vec3 mousePosition3D = gameObject->sim->mousePosition3D;
		glm::vec3 rayDirection = glm::normalize(mousePosition3D - rayOrigin);

		float planeY = 0.0f;
		float t = (planeY - rayOrigin.y) / rayDirection.y;
		glm::vec3 intersectionPoint = rayOrigin + t * rayDirection;

		LOG(INFO) << glm::to_string(intersectionPoint) << std::endl;

		gameObject->position = intersectionPoint;

	}
}