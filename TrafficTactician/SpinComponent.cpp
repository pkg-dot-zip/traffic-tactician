#include "SpinComponent.h"
#include "GameObject.h"


SpinComponent::SpinComponent(float speed)
{
	this->speed = speed;
}


SpinComponent::~SpinComponent()
{
}

void SpinComponent::update(float elapsedTime)
{
	gameObject->rotation.y += elapsedTime * speed;
}
