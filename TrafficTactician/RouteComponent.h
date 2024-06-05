#pragma once

#include "Component.h"
#include <vector>
#include "glm/ext/matrix_transform.hpp"
using glm::vec3;


class RouteComponent : public Component
{
	enum class RouteState {
		Idle,  // Not moving
		MovingFirst, // Following the first route
		MovingSecond, // Following the second route
		Finished  // Reached the last node
	};

	std::vector<vec3> nodesRoute1;
	std::vector<vec3> nodesRoute2;
	float speed;
	float tolerance = 0;
	std::vector<vec3> currentRoute;
	int currentWaypointIndex = 0;
public:
	RouteState state = RouteState::Idle;

	RouteComponent(
		float speed = 1, 
		std::vector<vec3> nodesRoute1 = std::vector<vec3>(),
		std::vector<vec3> nodesRoute2 = std::vector<vec3>()
	);
	~RouteComponent();

	virtual void update(float deltaTime) override;
};

