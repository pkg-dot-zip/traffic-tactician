#include "MoveComponent.h"
#include "GameObject.h"

MoveComponent::MoveComponent(const glm::vec3& direction)
{
	this->direction = direction;
}

void MoveComponent::nextDirection(const glm::vec3& direction)
{
	this->direction = direction;
}

void MoveComponent::update(float elapsedTime)
{
	gameObject->position += direction * elapsedTime;
}
