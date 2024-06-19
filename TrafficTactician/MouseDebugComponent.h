#pragma once

#include "Component.h"
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "tigl.h"
using tigl::Vertex;

class Simulation;

class MouseDebugComponent : public Component
{
	double lastSpawn = 0;
	double clickDelay = 3;
public:
	MouseDebugComponent();
	~MouseDebugComponent();

	std::vector<glm::vec3> points;

	bool allowPointClicks = false;

	virtual void update(float deltaTime) override;
};

