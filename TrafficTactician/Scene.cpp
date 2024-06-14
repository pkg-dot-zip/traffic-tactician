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
#include "SoundHandler.h"
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
	updateVisualCueTexture();
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
	routeCache[POSE_MOVE_RIGHT] = smoothPathWithBezier({
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(0.662630, 0.000000, 0.600000),
		glm::vec3(8, 0.000000, 0.600000)
		}, 20);

	routeCache[POSE_MOVE_LEFT] = smoothPathWithBezier({
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(-0.662630, 0.000000, -0.600000),
		glm::vec3(-8, 0.000000, -0.600000)
		}, 20);

	routeCache[POSE_MOVE_FORWARD] = {
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(-0.5, 0.000000, 6.000000)
	};

	routeCache[POSE_STOP] = {
		glm::vec3(-0.5, 0.000000, -6.864396),
		glm::vec3(-0.5, 0.000000, -1.55),
		glm::vec3(-0.5, 0.000000, 6.0),
	};
}

// Creates car with random pose.
std::shared_ptr<GameObject> Scene::createCar()
{
	return createCar(static_cast<Pose>(rand() % (POSE_OTHER))); // POSE_OTHER is the last enum value, however it is not one you should require the user to do. This is important!
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

	SoundHandler::getInstance().playSoundSnippet("sounds/car/Car_Acceleration_2.wav");
	return carObject;
}

glm::vec3 Scene::bezierPoint(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
	float t)
{
	float u = 1.0f - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	glm::vec3 point = (uuu * p0) + (3 * uu * t * p1) + (3 * u * tt * p2) + (ttt * p3);
	return point;
}

std::vector<glm::vec3> Scene::smoothPathWithBezier(const std::vector<glm::vec3>& points, int numPointsPerSegment)
{
	std::vector<glm::vec3> interpolatedPoints;
	constexpr float controlPointMultiplier = 0.2f;

	for (size_t i = 0; i < points.size() - 1; ++i) {
		glm::vec3 p0 = points[i];
		glm::vec3 p3 = points[i + 1];

		glm::vec3 direction1 = (i == 0) ? (points[i + 1] - points[i]) : (points[i + 1] - points[i - 1]);
		glm::vec3 direction2 = (i + 1 == points.size() - 1) ? (points[i + 1] - points[i]) : (points[i + 2] - points[i]);

		float distance1 = glm::length(p3 - p0);
		glm::vec3 p1 = p0 + controlPointMultiplier * distance1 * glm::normalize(direction1);

		float distance2 = glm::length(p3 - p0);
		glm::vec3 p2 = p3 - controlPointMultiplier * distance2 * glm::normalize(direction2);

		for (int j = 0; j <= numPointsPerSegment; ++j) {
			float t = static_cast<float>(j) / numPointsPerSegment;
			glm::vec3 point = bezierPoint(p0, p1, p2, p3, t);
			interpolatedPoints.push_back(point);
		}
	}

	// Ensure the last point is included
	interpolatedPoints.push_back(points.back());

	return interpolatedPoints;
}

void Scene::updateVisualCueTexture()
{
	// Then we set the correct texture for the visual cue sign on the topleft of the screen.
	std::string textureToRetrieve;
	switch (currentCarObject.lock()->getComponent<ControllerComponent>().value()->correctPose)
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

		updateVisualCueTexture();
	}
	else
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
