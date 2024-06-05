#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "GameObject.h"

#include "ModelComponent.h"
#include "WorldComponent.h"
#include "CarComponent.h"
#include "RouteComponent.h"


Scene::Scene(Simulation* sim, int worldSize)
{
	this->sim = sim;
	initWorld(worldSize);

	// Create a car object at 0,0,0
	car = std::make_shared<GameObject>("car", sim);
	car->scale = 0.4f * car->scale;

	// TOP to halt point nodes
	std::vector<glm::vec3> topToDownNodes = {
		glm::vec3(0.591764, 0.000000, -6.864396),
		glm::vec3(0.549422, 0.000000, -5.504863),
		glm::vec3(0.609607, 0.000000, -4.175739),
		glm::vec3(0.676493, 0.000000, -1.55)
	};

	// halt to RIGHT point nodes
	// TODO: create list of nodes for all directions
	std::vector<glm::vec3> haltToLeftNodes = {
		glm::vec3(0.662630, 0.000000, -1.543589),
		glm::vec3(0.662630, 0.000000, 0.543589),
		glm::vec3(0.662630, 0.000000, 0.600000),
		glm::vec3(0.781084, 0.000000, 0.600000),
		glm::vec3(1.420864, 0.000000, 0.600000),
		glm::vec3(2.232835, 0.000000, 0.600000),
		glm::vec3(3.183463, 0.000000, 0.600000),
		glm::vec3(5, 0.000000, 0.600000),
	};


	car->position = haltToLeftNodes.front();
	car->addComponent(std::make_shared<ModelComponent>("models/car_kit/ambulance.obj"));
	//car->addComponent(std::make_shared<CarComponent>());
	float speed = 1.5;
	car->addComponent(std::make_shared<RouteComponent>(speed, haltToLeftNodes));
	objects.push_back(car);
}

void Scene::initWorld(int worldSize)
{
	std::shared_ptr<GameObject> worldObject = std::make_shared<GameObject>("world", sim);
	worldObject->scale = 4.0f * worldObject->scale;
	std::shared_ptr<WorldComponent> world_component = std::make_shared<WorldComponent>(worldSize, 1.0f, std::make_shared<ModelComponent>("models/road_kit/tile_low.obj"));
	for (int i = 0; i < worldSize; i++)
	{
		if (i == worldSize / 2)
			continue;

		world_component->setModel(worldSize / 2, i, std::make_shared<ModelComponent>("models/road_kit/road_straight_rotated.obj"));
		world_component->setModel(i, worldSize / 2, std::make_shared<ModelComponent>("models/road_kit/road_straight.obj"));
	}

	world_component->setModel(worldSize / 2, worldSize / 2, std::make_shared<ModelComponent>("models/road_kit/road_crossroad.obj"));
	worldObject->addComponent(world_component);

	objects.push_back(worldObject);
}


void Scene::update(float deltaTime)
{
	for (auto& o : objects) {
		o->update(deltaTime);
	}
}

void Scene::draw()
{
	for (auto& o : objects) {
		o->draw();
	}
}


