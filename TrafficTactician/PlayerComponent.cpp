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

void PlayerComponent::update(float elapsedTime)
{

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gameObject->position.z -= elapsedTime * speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gameObject->position.z += elapsedTime * speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gameObject->position.x -= elapsedTime * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gameObject->position.x += elapsedTime * speed;

}
