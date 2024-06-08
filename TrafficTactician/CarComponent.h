#pragma once

#include "Component.h"
#include <vector>
#include "tigl.h"
using tigl::Vertex;

class Simulation;

class CarComponent : public Component
{
public:
	CarComponent() = default;
	~CarComponent() = default;

	std::vector<glm::vec3> points;

	bool allowPointClicks = false;

	virtual void update(float deltaTime) override;
private:
	double lastSpawn = 0;
	double clickDelay = 4;
};
