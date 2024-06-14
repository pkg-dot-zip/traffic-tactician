#include "RouteComponent.h"
#include "GameObject.h"
#include <easylogging++.h>
#include <glm/gtx/string_cast.hpp>

#include "SoundHandler.h"


//// Function to compute a point on a cubic Bezier curve
//glm::vec3 bezierPoint(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
//	float u = 1.0f - t;
//	float tt = t * t;
//	float uu = u * u;
//	float uuu = uu * u;
//	float ttt = tt * t;
//
//	glm::vec3 point = (uuu * p0) + (3 * uu * t * p1) + (3 * u * tt * p2) + (ttt * p3);
//	return point;
//}
//
//// Function to generate a smoothed path using cubic Bezier curves with adjusted control points
//std::vector<glm::vec3> bezierCurve(const std::vector<glm::vec3>& points, int numPointsPerSegment) {
//	std::vector<glm::vec3> interpolatedPoints;
//	float controlPointMultiplier = 0.2f;
//
//	for (size_t i = 0; i < points.size() - 1; ++i) {
//		glm::vec3 p0 = points[i];
//		glm::vec3 p3 = points[i + 1];
//
//		glm::vec3 direction1 = (i == 0) ? (points[i + 1] - points[i]) : (points[i + 1] - points[i - 1]);
//		glm::vec3 direction2 = (i + 1 == points.size() - 1) ? (points[i + 1] - points[i]) : (points[i + 2] - points[i]);
//
//		float distance1 = glm::length(p3 - p0);
//		glm::vec3 p1 = p0 + controlPointMultiplier * distance1 * glm::normalize(direction1);
//
//		float distance2 = glm::length(p3 - p0);
//		glm::vec3 p2 = p3 - controlPointMultiplier * distance2 * glm::normalize(direction2);
//
//		for (int j = 0; j <= numPointsPerSegment; ++j) {
//			float t = float(j) / numPointsPerSegment;
//			glm::vec3 point = bezierPoint(p0, p1, p2, p3, t);
//			interpolatedPoints.push_back(point);
//		}
//	}
//
//	// Ensure the last point is included
//	interpolatedPoints.push_back(points.back());
//
//	return interpolatedPoints;
//}

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