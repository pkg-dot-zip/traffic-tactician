#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
	Scene* scene;
	glm::vec3 mousePosition3D;

	Simulation(GLFWwindow* window);
	void init();
	void update(float deltaTime);
	void draw();

	glm::vec3 getCameraPosition();
};

