#pragma once

#include <vector>
#include <memory>
#include <map>
#include "GameObject.h"
#include "Simulation.h"
#include "poseChecker.h"
class Scene
{
	void initWorld(int worldSize);
	void initRouteCache();
	std::shared_ptr<GameObject> createCar(Pose pose);
	std::map<Pose, std::vector<glm::vec3>> routeCache;
public: 
	Simulation* sim;
	std::vector<std::shared_ptr<GameObject>> objects;
	std::shared_ptr<GameObject> currentCarObject;


	Scene(Simulation* sim, int worldSize = 11);
	void update(float deltaTime);
	void draw();

	
};

