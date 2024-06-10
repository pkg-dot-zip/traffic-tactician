#include <GL/glew.h>

#include "Scene.h"

#include <ostream>

#include "GameObject.h"

#include "ModelComponent.h"
#include "WorldComponent.h"
#include "RouteComponent.h"
#include "ControllerComponent.h"
#include "easylogging++.h"
#include "RandomModelGrabber.h"
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

	// Init first car.
	objects.push_back(createCar());
	currentCarObject = objects.back();
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
	routeCache[Pose::POSE_MOVE_RIGHT] = {
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(0.662630, 0.000000, 0.600000),
		glm::vec3(8, 0.000000, 0.600000)
	};

	// TODO: Make sure the car drives on the other lane!
	routeCache[Pose::POSE_MOVE_LEFT] = {
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(-0.662630, 0.000000, 0.600000),
		glm::vec3(-8, 0.000000, 0.600000)
	};

	routeCache[Pose::POSE_MOVE_FORWARD] = {
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(-0.5, 0.000000, 0.600000),
		glm::vec3(-0.5, 0.000000, 6.000000)
	};

	// TODO: Add route and support for POSE_STOP
}

// Creates car with random pose.
std::shared_ptr<GameObject> Scene::createCar()
{
	const Pose randomPose = static_cast<Pose>(rand() % (Pose::POSE_OTHER - 1)); // POSE_OTHER is the last enum value, however it is not one you should require the user to do. This is important!
	return createCar(randomPose);
}

// Create a car object with the given pose.
std::shared_ptr<GameObject> Scene::createCar(Pose pose)
{
	auto carObject = std::make_shared<GameObject>("car", sim);
	carObject->scale = 0.4f * carObject->scale;

	carObject->addComponent(std::make_shared<ModelComponent>(randomModelGrabber::getRandomCarModelPathAsString()));

	std::vector<glm::vec3> route = routeCache[pose];
	carObject->position = route.front(); // Set spawn point to the first node.

	constexpr float speed = 1.5;
	carObject->addComponent(std::make_shared<RouteComponent>(speed, route));
	carObject->addComponent(std::make_shared<ControllerComponent>(pose, this));

	return carObject;
}


void Scene::update(float deltaTime)
{
	// First update all the gameObjects.
	for (const auto& o : objects) {
		o->update(deltaTime);
	}

	// Then update the car spawning mechanism.
	if (currentCarObject.expired())
	{
		LOG(INFO) << "No cars currently in the scene. Spawning new one." << std::endl;
		objects.push_back(createCar());
		currentCarObject = objects.back();
		LOG(INFO) << "Spawned new car in scene." << std::endl;

		// Then we set the correct texture for the visual cue sign on the topleft of the screen.
		std::string textureToRetrieve;
		Pose poseToDo = currentCarObject.lock()->getComponent<ControllerComponent>().value()->correctPose;
		switch (poseToDo)
		{
		case POSE_MOVE_RIGHT:
			textureToRetrieve = "rightSign";
			break;
		case POSE_MOVE_LEFT:
			textureToRetrieve = "leftSign";
			break;
		case POSE_MOVE_FORWARD:
			textureToRetrieve = "forwardSign";
			break;
		case POSE_STOP:
			textureToRetrieve = "stopSign";
			break;
		case POSE_OTHER:
			LOG(WARNING) << "Looking for texture for POSE_OTHER. This should not happen. Did you intend this?" << std::endl;
			break;
		default:
			LOG(ERROR) << "No sign texture implemented for current pose. Throwing." << std::endl;
			throw std::exception("No sign texture implemented for current pose. Throwing.");
		}

		data.currentSignTexture = &data.textures[textureToRetrieve];
	} else
	{
		const std::shared_ptr<GameObject> carGameObject = currentCarObject.lock();
		if (carGameObject->getComponent<RouteComponent>().value()->state == RouteComponent::RouteState::Finished)
		{
			std::erase(objects, carGameObject);
			currentCarObject.reset();
			LOG(INFO) << "Removed current car from the scene because the car finished its route." << std::endl;
		}
	}
}

void Scene::draw() const
{
	for (const auto& o : objects) {
		o->draw();
	}
}
