#include <GL/glew.h>

#include "Scene.h"
#include "GameObject.h"

#include "ModelComponent.h"
#include "WorldComponent.h"
#include "RouteComponent.h"
#include "ControllerComponent.h"
#include "Texture.h"
#include "TextureCache.h"


Scene::Scene(const std::weak_ptr<Simulation>& sim, int worldSize)
{
	this->sim = sim;
	initRouteCache();
	initWorld(worldSize);

	data.textures["scoreLogo"] = TextureCache::loadTexture("score_logo.png")->id;
	data.textures["stopSign"] = TextureCache::loadTexture("sign_stop.png")->id;
	data.textures["forwardSign"] = TextureCache::loadTexture("sign_forward.png")->id;
	data.textures["leftSign"] = TextureCache::loadTexture("sign_left.png")->id;
	data.textures["rightSign"] = TextureCache::loadTexture("sign_right.png")->id;
	data.currentSignTexture = &data.textures["stopSign"];

	// Create a car object with the given pose.
	currentCarObject = createCar(POSE_MOVE_RIGHT);
	objects.push_back(currentCarObject);
}

void Scene::initWorld(int worldSize)
{
	const std::shared_ptr<GameObject> worldObject = std::make_shared<GameObject>("world", sim);
	worldObject->scale = 4.0f * worldObject->scale;
	const std::shared_ptr<WorldComponent> world_component = std::make_shared<WorldComponent>(worldSize, 1.0f, std::make_shared<ModelComponent>("models/road_kit/tile_low.obj"));
	for (int i = 0; i < worldSize; i++)
	{
		if (i == worldSize / 2) continue;

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

	// TODO: Add random car model.
	carObject->addComponent(std::make_shared<ModelComponent>("models/car_kit/ambulance.obj"));


	// TODO: Add more routes.
	std::vector<glm::vec3> route = routeCache[Pose::POSE_MOVE_RIGHT];
	carObject->position = route.front(); // set spawn point to the first node

	constexpr float speed = 1.5;
	carObject->addComponent(std::make_shared<RouteComponent>(speed, route));

	carObject->addComponent(std::make_shared<ControllerComponent>(pose, this));

	return carObject;
}


void Scene::update(float deltaTime) const
{
	for (const auto& o : objects) {
		o->update(deltaTime);
	}
}

void Scene::draw() const
{
	for (auto& o : objects) {
		o->draw();
	}
}
