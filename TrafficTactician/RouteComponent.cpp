#include "RouteComponent.h"
#include "GameObject.h"
#include <easylogging++.h>
#include <glm/gtx/string_cast.hpp>

#include "SoundHandler.h"

RouteComponent::RouteComponent(float speed, const std::vector<glm::vec3>& nodesRoute)
{
	this->speed = speed;
	this->currentRoute = nodesRoute;

	state = RouteState::Moving;
}

void RouteComponent::update(float deltaTime) {

	if (state == RouteState::Idle || state == RouteState::Finished || currentRoute.empty()) return;

	if (state != RouteState::Moving) return;

	constexpr glm::vec3 center = glm::vec3(0.0f);
	const float distanceToCenter = glm::abs(glm::length(center - gameObject->position));
	constexpr float range = 2.0f;
	if (distanceToCenter < range && !crossed) {
		state = RouteState::Idle;
		SoundHandler::getInstance().playSoundSnippet("sounds/car/Car_Parking_Brake.wav");
		return;
	}

	// Get the current waypoint and object's position.
	const glm::vec3 currentWaypoint = currentRoute[currentWaypointIndex];
	const glm::vec3 currentPosition = gameObject->position;

	// Calculate the direction vector towards the waypoint.
	const glm::vec3 direction = glm::normalize(currentWaypoint - currentPosition);

	// Move the object towards the waypoint based on speed and deltaTime.
	const float distance = glm::length(currentWaypoint - currentPosition);
	if (distance > tolerance) { // Check if within tolerance (optional).
		float movement = speed * deltaTime;
		if (movement > distance) {
			movement = distance; // Avoid overshooting the waypoint.
		}

		gameObject->position += movement * direction;

		// to create a "turning motion".
		const float targetAngle = glm::atan(direction.x, direction.z);
		gameObject->rotation.y = targetAngle;
	}
	else {
		// Reached the waypoint, handle waypoint change (optional)
		currentWaypointIndex++;
		if (currentWaypointIndex >= currentRoute.size()) {
			// Handle reaching the end of the route (loop, stop, etc.)
			currentRoute.clear();
			state = RouteState::Finished;
			SoundHandler::getInstance().playSoundSnippet("sounds/car/Car_Engine_Turning_Off.wav");
			return;
		}
	}
}