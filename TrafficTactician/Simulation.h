#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "SettingsRetriever.h"

class Scene;

class Simulation 
{
public:
	std::unique_ptr<Scene> scene;
	glm::vec3 mousePosition3D;

	Simulation(GLFWwindow* window);
	void init(const std::weak_ptr<Simulation>& sim);
	void update(float deltaTime) const;
	void draw();

	glm::vec3 getCameraPosition() const;
private:
	GLFWwindow* window;
	int width = GetGraphicSettings().screenWidth;
	int height = GetGraphicSettings().screenHeight;
	glm::mat4 projection;
	glm::mat4 view;
};
