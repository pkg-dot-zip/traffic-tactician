#include "RouteComponent.h"
#include "GameObject.h"
#include <easylogging++.h>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

RouteComponent::RouteComponent(float speed, std::vector<vec3> nodes)
{
    this->speed = speed;
    this->nodes = nodes;
}

RouteComponent::~RouteComponent()
{
}

void RouteComponent::update(float deltaTime) {
    if (!setMoving) return;

    // Get the current waypoint and object's position
    glm::vec3 currentWaypoint = nodes[currentWaypointIndex];
    glm::vec3 currentPosition = gameObject->position;

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
        gameObject->position.y = 0;
    }
    else {
        // Reached the waypoint, handle waypoint change (optional)
        currentWaypointIndex++;
        if (currentWaypointIndex >= nodes.size()) {
            // Handle reaching the end of the route (loop, stop, etc.)
            setMoving = false;
            nodes.clear();
        }
    }
    LOG(INFO) << "Car at " << glm::to_string(gameObject->position) << "\n";
}
