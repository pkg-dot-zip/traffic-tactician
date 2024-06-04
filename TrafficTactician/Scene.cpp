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
	std::shared_ptr<GameObject> car = std::make_shared<GameObject>("car", sim);
	car->scale = 0.4f * car->scale;
	
	// TOP to halt point nodes
	std::vector<glm::vec3> topToDownNodes = { 
	vec3(0.538546, 0.000000, -6.842329),
	vec3(0.518869, 0.000000, -6.149068),
	vec3(0.616513, 0.000000, -4.360466),
	vec3(0.616513, 0.000000, -4.360466),
	vec3(0.629492, 0.000000, -2.077454)
	};

	// halt to RIGHT point nodes
	// TODO: create list of nodes for all directions
	std::vector<glm::vec3> haltToLeftNodes = {
	vec3(0.629492, 0.000000, -2.077454),
	vec3(0.65, 0.000000, -1.5),
	vec3(0.65, 0.000000, -1.0),
	};

	
	car->position = haltToLeftNodes.front();
	car->addComponent(std::make_shared<ModelComponent>("models/car_kit/ambulance.obj"));
	//car->addComponent(std::make_shared<CarComponent>());
	float speed = 1;
	car->addComponent(std::make_shared<RouteComponent>(speed, haltToLeftNodes));
	objects.push_back(car);
}

void Scene::initWorld(int worldSize)
{
	std::shared_ptr<GameObject> worldObject = std::make_shared<GameObject>("world", sim);
	worldObject-> scale = 4.0f * worldObject->scale;
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


