#include "ControllerComponent.h"
#include "GameObject.h"
#include "RouteComponent.h"
#include "InputHandler.h"
#include <iostream>
#include <memory>

#include "easylogging++.h"
#include "Scene.h"
#include "Timer.h"

ControllerComponent::ControllerComponent(Pose pose, Scene* scene) : scene(scene), correctPose(pose)
{
	this->timer = std::make_shared<Timer>([this] { timerCallback(); });
}

bool ControllerComponent::checkPose() const
{
	return getInputPose() == correctPose;
}

void ControllerComponent::timerCallback() const
{
	if (!checkPose())
	{
		scene->data.points--;
		return;
	}

	if (gameObject->getComponent<RouteComponent>().has_value())
	{
		gameObject->getComponent<RouteComponent>().value()->crossed = true;
		gameObject->getComponent<RouteComponent>().value()->state = RouteComponent::RouteState::Moving;
	}
	else
	{
		LOG(ERROR) << "Error: Can not update UI when no RouteComponent can be found." << std::endl;
		throw std::exception("Error: Can not update UI when no RouteComponent can be found.");
	}

	timer->toggleTimer(false);
	scene->data.points++;
}

void ControllerComponent::update(float deltaTime)
{
	timer->update(deltaTime);

	// Update overlay data
	scene->data.remainingTime = timer->getTimeRemaining();
	scene->data.progress = timer->getTimeRemaining() / timer->rolloverTime;
}
