#include "PlayerComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>

extern GLFWwindow* window;


PlayerComponent::PlayerComponent()
{
}

PlayerComponent::~PlayerComponent()
{
}

void PlayerComponent::update(float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gameObject->position.z -= deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gameObject->position.z += deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gameObject->position.x -= deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gameObject->position.x += deltaTime * speed;
}
