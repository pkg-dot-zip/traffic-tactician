#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "GameObject.h"

#include "ModelComponent.h"
#include "WorldComponent.h"
#include "CarComponent.h"


void Scene::initWorld(int worldSize)
{
	std::shared_ptr<GameObject> worldObject = std::make_shared<GameObject>("world", sim);
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


Scene::Scene(Simulation* sim, int worldSize)
{
	this->sim = sim;
	initWorld(worldSize);
	
	// Create a car object at 0,0,0
	std::shared_ptr<GameObject> car = std::make_shared<GameObject>("car", sim);
	car->position = glm::vec3(0, 0, 0);
	car->addComponent(std::make_shared<ModelComponent>("models/car_kit/ambulance.obj"));
	car->addComponent(std::make_shared<CarComponent>());

	objects.push_back(car);
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


