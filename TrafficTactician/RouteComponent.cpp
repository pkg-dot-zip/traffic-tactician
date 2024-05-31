#include "RouteComponent.h"
#include "GameObject.h"

RouteComponent::RouteComponent(float speed)
{
    this->speed = speed;
	routeNodes = {
		vec3(1.081668, 0.000000, -6.142860),
		vec3(1.111798, 0.000000, -3.522040),
		vec3(1.224573, 0.000000, -1.149999),
		vec3(3.659592, 0.000000, -1.280130)
	};
}

RouteComponent::~RouteComponent()
{
}

void RouteComponent::update(float elapsedTime)
{


    //float xDistance = position.x - gameObject->position.x;
    //float yDistance = position.y - gameObject->position.y;
    //float zDistance = position.z - gameObject->position.z;

    //// Calculate steps, but first check for zero distances to avoid division by zero
    //float xStep = (xDistance != 0) ? (xDistance / std::abs(xDistance)) * elapsedTime * speed : 0;
    //float yStep = (yDistance != 0) ? (yDistance / std::abs(yDistance)) * elapsedTime * speed : 0;
    //float zStep = (zDistance != 0) ? (zDistance / std::abs(zDistance)) * elapsedTime * speed : 0;

    ////update x coördinate
    //if (std::abs(xDistance) < std::abs(xStep)) {
    //    gameObject->position.x = position.x;
    //}
    //else {
    //    gameObject->position.x += xStep;
    //}

    ////update y coördinate
    //if (std::abs(yDistance) < std::abs(yStep)) {
    //    gameObject->position.y = position.y;
    //}
    //else {
    //    gameObject->position.y += yStep;
    //}

    ////update z coördinate
    //if (std::abs(zDistance) < std::abs(zStep)) {
    //    gameObject->position.z = position.z;
    //}
    //else {
    //    gameObject->position.z += zStep;
    //}
}
