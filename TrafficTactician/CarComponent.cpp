#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "CarComponent.h"
#include "GameObject.h"
#include "Simulation.h"
#include <easylogging++.h>
#include <glm/gtx/string_cast.hpp>


extern GLFWwindow* window;

void CarComponent::update(float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		allowPointClicks = !allowPointClicks;
		LOG(INFO) << "Clicks allowed" << std::endl;
	}

	if (!allowPointClicks) return;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && lastSpawn + clickDelay < glfwGetTime())
	{
		const auto sim = this->gameObject->sim.lock();
		const glm::vec3 rayOrigin = sim->getCameraPosition();
		const glm::vec3 mousePosition3D = sim->mousePosition3D;
		const glm::vec3 rayDirection = glm::normalize(mousePosition3D - rayOrigin);

		constexpr float planeY = 0.0f;
		const float t = (planeY - rayOrigin.y) / rayDirection.y;
		const glm::vec3 intersectionPoint = rayOrigin + t * rayDirection;

		LOG(INFO) << glm::to_string(intersectionPoint) << std::endl;

		this->gameObject->position = intersectionPoint;
	}
}
