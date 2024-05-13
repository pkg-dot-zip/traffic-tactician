#include "SpinComponent.h"
#include "GameObject.h"


SpinComponent::SpinComponent(float speed = 1)
{
	this->speed = speed;
}

SpinComponent::~SpinComponent() = default;

void SpinComponent::update(float deltaTime)
{
	gameObject->rotation.y += deltaTime * speed;
}