#include <GL/glew.h>
#include "Simulation.h"

#include <ostream>

#include "Scene.h"
#include "tigl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "easylogging++.h"

Simulation::Simulation(GLFWwindow* window)
{
	this->window = window;
	mousePosition3D = glm::vec3();
	projection = glm::mat4();
	view = glm::mat4();
}

void Simulation::init(const std::weak_ptr<Simulation>& sim)
{
	scene = std::make_unique<Scene>(sim);
	LOG(INFO) << "Initialized simulation." << std::endl;
}

void Simulation::update(float deltaTime) const
{
	scene->update(deltaTime);
}

void Simulation::draw()
{
	std::array<int, 4> viewport;
	glGetIntegerv(GL_VIEWPORT, viewport.data());
	projection = glm::perspective(glm::radians(75.0f), viewport[2] / static_cast<float>(viewport[3]), 0.01f, 10.0f);
	view = glm::lookAt(glm::vec3(0, 4.5f, 2), glm::vec3(0), glm::vec3(0, 1, 0));
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(view);
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	scene->draw();

	// Calculate mousePosition3D.
	glm::vec3 mouse2D;
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);
	mouse2D.x = static_cast<float>(xpos);
	mouse2D.y = viewport[3] - static_cast<float>(ypos);
	glReadPixels(static_cast<int>(xpos), viewport[3] - static_cast<int>(ypos), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse2D.z);

	mousePosition3D = glm::unProject(
		mouse2D,
		view,
		projection,
		glm::uvec4(viewport[0], viewport[1], viewport[2], viewport[3]));
}

// Extract camera position from the inverse of the view matrix.
glm::vec3 Simulation::getCameraPosition() const
{
	glm::mat4 inverseView = glm::inverse(view);
	const glm::vec3 cameraPosition = glm::vec3(inverseView[3]);
	return cameraPosition;
}
