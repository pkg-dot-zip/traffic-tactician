#pragma once

#include "Component.h"
#include <vector>
#include "glm/ext/matrix_transform.hpp"
using glm::vec3;


class RouteComponent : public Component
{
	std::vector<vec3> nodes;
	float speed;
	float tolerance = 0;
	int currentWaypointIndex = 0;
	bool setMoving = true;
public:
	RouteComponent(float speed = 1, std::vector<vec3> nodes = std::vector<vec3>());
	~RouteComponent();

	virtual void update(float deltaTime) override;
};

