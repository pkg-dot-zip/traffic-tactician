#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "GameObject.h"

#include "ModelComponent.h"
#include "WorldComponent.h"
#include "RouteComponent.h"
#include "ControllerComponent.h"


Scene::Scene(Simulation* sim, int worldSize)
{
	this->sim = sim;
	initRouteCache();
	initWorld(worldSize);
	
	// Create a car object with the given pose
	currentCarObject = createCar(Pose::POSE_MOVE_RIGHT);
	objects.push_back(currentCarObject);
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

void Scene::initRouteCache()
{
	// TOP to halt point nodes
	routeCache[Pose::POSE_MOVE_RIGHT] = {
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(0.662630, 0.000000, 0.600000),
		glm::vec3(8, 0.000000, 0.600000)
	};

	// TODO: add more routes
}

// TODO: add more routes
// TODO: add random car model
// Create a car object with the given pose
std::shared_ptr<GameObject> Scene::createCar(Pose pose)
{
	auto carObject = std::make_shared<GameObject>("car", sim);
	carObject->scale = 0.4f * carObject->scale;

	// TODO: add random car model
	carObject->addComponent(std::make_shared<ModelComponent>("models/car_kit/ambulance.obj"));


	// TODO: add more routes
	std::vector<glm::vec3> route = routeCache[Pose::POSE_MOVE_RIGHT];
	carObject->position = route.front(); // set spawn point to the first node

	float speed = 1.5;
	carObject->addComponent(std::make_shared<RouteComponent>(speed, route));

	carObject->addComponent(std::make_shared<ControllerComponent>(pose));

	return carObject;
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


