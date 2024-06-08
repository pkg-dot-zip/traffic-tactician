#include "ControllerComponent.h"
#include "GameObject.h"
#include "RouteComponent.h"
#include "InputHandler.h"
#include <iostream>
#include <memory>

#include "easylogging++.h"
#include "Scene.h"
#include "Timer.h"

ControllerComponent::ControllerComponent(Pose pose, Scene* scene) : correctPose(pose), scene(scene)
{
	this->timer = std::make_shared<Timer>();

	timer->setCallback([this] { timerCallback(); });
}

bool ControllerComponent::checkPose()
{
	Pose pose = getInputPose();

	return (pose == correctPose);
}

void ControllerComponent::timerCallback()
{
	if (!checkPose())
	{
		scene->data.points--;
		return;
	}

	gameObject->getComponent<RouteComponent>()->crossed = true;
	gameObject->getComponent<RouteComponent>()->state = RouteComponent::RouteState::Moving;

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
