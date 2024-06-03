#include "RouteComponent.h"
#include "GameObject.h"
#include <easylogging++.h>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

RouteComponent::RouteComponent(float speed, vec3 position)
{
    this->speed = speed;
    this->direction = position;

    routeNodes = {
		vec3(0.5, 0.000000, -6.142860),
		vec3(0.5, 0.000000, -3.522040),
		vec3(0.5, 0.000000, -1.149999)
	};

}

RouteComponent::~RouteComponent()
{
}

void RouteComponent::update(float deltaTime) {
    // Epsilon for reaching node tolerance
    float epsilon = 0.1f;

    // Check if reached the target node with tolerance
    if (glm::distance(gameObject->position, direction) < epsilon) {
        if (!routeNodes.empty()) {
            direction = routeNodes.front();
            routeNodes.erase(routeNodes.begin()); // Remove reached node
        }
        else {
            return;
        }
    }

    // Calculate distance to target and max movement per update
    float distance = glm::distance(gameObject->position, direction);
    float maxStep = speed * deltaTime;

    // Calculate movement vector with capped magnitude
    glm::vec3 movement = glm::normalize(direction - gameObject->position) * min(distance, maxStep);

    // Update position with smooth movement
    gameObject->position += movement;

    LOG(INFO) << "Car at " << glm::to_string(gameObject->position) << "\n";
}
