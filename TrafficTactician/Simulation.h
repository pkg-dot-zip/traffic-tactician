#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Scene.h"

class Scene;

class Simulation 

{
private:
	GLFWwindow* window;
	int width = 1600;
	int height = 900;
	glm::mat4 projection;
	glm::mat4 view;
public:
	std::unique_ptr<Scene> scene;
	glm::vec3 mousePosition3D;

	Simulation(GLFWwindow* window);
	void init(const std::weak_ptr<Simulation>& sim);
	void update(float deltaTime) const;
	void draw();

	glm::vec3 getCameraPosition() const;
};

