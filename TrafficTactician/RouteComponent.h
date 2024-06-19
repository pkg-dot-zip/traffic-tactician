#pragma once

#include "Component.h"
#include <vector>
#include "glm/ext/matrix_transform.hpp"
using glm::vec3;

class RouteComponent : public Component
{
public:
	enum class RouteState {
		Idle,  // Not moving
		Moving, // Moving
		Finished  // Reached the last node
	};
	RouteState state = RouteState::Idle;
	bool crossed = false;

	RouteComponent(
		float speed = 1,
		const std::vector<vec3>& nodesRoute = std::vector<vec3>()
	);

	~RouteComponent() = default;

	virtual void update(float deltaTime) override;

private:
	float speed;
	float tolerance = 0;
	std::vector<vec3> currentRoute;
	int currentWaypointIndex = 0;
};
