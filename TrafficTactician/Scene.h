#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"
#include "Simulation.h"


class Scene
{
	void initWorld(int worldSize);
public: 
	Simulation* sim;
	std::vector<std::shared_ptr<GameObject>> objects;
	std::shared_ptr<GameObject> getGameObject(const std::string& name);
	std::shared_ptr<GameObject> car;

	Scene(Simulation* sim, int worldSize = 11);
	void update(float deltaTime);
	void draw();

	
};

