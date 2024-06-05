#include "RouteComponent.h"
#include "GameObject.h"
#include <easylogging++.h>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

RouteComponent::RouteComponent(float speed, std::vector<vec3> nodesRoute)
{
	this->speed = speed;
	this->currentRoute = nodesRoute;

	state = RouteState::Moving;
}

RouteComponent::~RouteComponent()
{
}

void RouteComponent::update(float deltaTime) {

	if (state == RouteState::Idle || state == RouteState::Finished || currentRoute.empty()) return;

	// check if allowed to continue on the second route
	if (state != RouteState::Moving) return;

	// check if object is in radius to center of crossroads
	vec3 center = vec3(0.0f, 0.0f, 0.0f);
	float distanceToCenter = std::abs(glm::length(center - gameObject->position));
	float range = 2.0f;
	if (distanceToCenter < range && !crossed) {
		state = RouteState::Idle;
		return;
	}

	// Get the current waypoint and object's position
	glm::vec3 currentWaypoint = currentRoute[currentWaypointIndex];
	glm::vec3 currentPosition = gameObject->position;
	LOG(INFO) << "Car at " << glm::to_string(gameObject->position) << "\n";

	// Calculate the direction vector towards the waypoint
	glm::vec3 direction = glm::normalize(currentWaypoint - currentPosition);

	// Move the object towards the waypoint based on speed and deltaTime
	float distance = glm::length(currentWaypoint - currentPosition);
	if (distance > tolerance) { // Check if within tolerance (optional)
		float movement = speed * deltaTime;
		if (movement > distance) {
			movement = distance; // Avoid overshooting the waypoint
		}

		gameObject->position = vec3(currentPosition + movement * direction);

		// change the angle according to the direction vector
		// to create a "turning motion"
		float targetAngle = glm::atan(direction.x, direction.z);
		gameObject->rotation.y = targetAngle;
	}
	else {
		// Reached the waypoint, handle waypoint change (optional)
		currentWaypointIndex++;
		if (std::distance(currentRoute.begin(), currentRoute.end()) <= currentWaypointIndex) {
			// Handle reaching the end of the route (loop, stop, etc.)
				currentRoute.clear();
				state = RouteState::Finished;
				return;
		}
	}
}
