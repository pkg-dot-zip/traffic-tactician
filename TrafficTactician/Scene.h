#pragma once

#include <vector>
#include <memory>
#include <map>
#include "GameObject.h"
#include "Simulation.h"
#include "poseChecker.h"

class Scene
{
public:
	std::weak_ptr<Simulation> sim;
	std::vector<std::shared_ptr<GameObject>> objects;
	std::weak_ptr<GameObject> currentCarObject;

	struct OverlayData
	{
		int points = 0;
		float remainingTime = 0.0f;
		float progress = 0.0f;
		GLuint* currentSignTexture = nullptr; // TODO: Change this to a pointer to the current sign texture e.g. currentSignTexture = &textures["stopSign"];
		std::map<std::string, GLuint> textures; // scoreLogo, stopSign, forwardSign, leftSign, rightSign
	};

	OverlayData data;


	Scene(const std::weak_ptr<Simulation>& sim, int worldSize = 11);
	void update(float deltaTime);
	void draw() const;
private:
	std::map<Pose, std::vector<glm::vec3>> routeCache;
	void initWorld(int worldSize);
	void initRouteCache();
	void updateVisualCueTexture();
	std::shared_ptr<GameObject> createCar();
	std::shared_ptr<GameObject> createCar(Pose pose);
};
