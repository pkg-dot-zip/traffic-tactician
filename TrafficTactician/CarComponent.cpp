#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "CarComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"


extern GLFWwindow* window;
extern std::vector<tigl::Vertex> verts;




CarComponent::CarComponent()
{
}

CarComponent::~CarComponent()
{
}

void CarComponent::update(float elapsedTime)
{
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
	//	allowPointClicks = !allowPointClicks;
	//}

	//if (!allowPointClicks) return;

	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && lastSpawn + clickDelay < glfwGetTime())
	//{
	//	auto direction = mousePosition3D;
	//	direction.y = 0;
	//	direction = glm::normalize(direction);


	//	if (!points.empty()) {
	//		glm::vec3 lastElement = points.back();
	//		if (direction == lastElement) return;
	//	}

	//	points.push_back(direction);
	//	gameObject->position = direction;
		/*gameObject->getCo(gameObject->position, direction);*/

		/*glm::vec4 color = glm::vec4(1,1,1,1);
		verts.clear();
		for (glm::vec3 point : points) {
			Vertex v = Vertex::PC(point, color);
			verts.push_back(v);
		}*/
}



void CarComponent::draw(glm::mat4 parentMatrix)
{
	tigl::drawVertices(GL_TRIANGLES, verts);
	/*glm::vec3 oldPoint = gameObject->position;
	glm::vec4 color(1, 1, 1, 1);
	for (glm::vec3 point : points) {
		Vertex v1 = Vertex::PC(oldPoint, color);
		Vertex v2 = Vertex::PC(point, color);
		tigl::drawVertices(GL_LINE, {v1, v2});
		oldPoint = point;
	}*/
}
