#include "ControllerComponent.h"
#include "GameObject.h"
#include "CameraInputHandler.h"
#include <iostream>
#include <memory>

#include "easylogging++.h"
#include "Scene.h"
#include "SoundHandler.h"
#include "Timer.h"

ControllerComponent::ControllerComponent(Pose pose, Scene* scene) : scene(scene), correctPose(pose)
{
	this->timer = std::make_shared<Timer>([this] { timerCallback(); });
}

bool ControllerComponent::checkPose() const
{
	return cameraInputHandler::getInputPose() == correctPose;
}

void ControllerComponent::changeCarState(RouteComponent::RouteState state) const
{
	if (gameObject->getComponent<RouteComponent>().has_value())
	{
		gameObject->getComponent<RouteComponent>().value()->crossed = true;
		gameObject->getComponent<RouteComponent>().value()->state = state;
	}
	else
	{
		LOG(ERROR) << "Error: Can not update UI when no RouteComponent can be found." << std::endl;
		throw std::exception("Error: Can not update UI when no RouteComponent can be found.");
	}
}

void ControllerComponent::timerCallback() const
{
	// Timer has ended before a correct pose was detected
	int& points = scene->data.points;
	if (points > 0) points--;
	SoundHandler::getInstance().playSoundSnippet("sounds/points_minus.wav");
	return;
}

void ControllerComponent::update(float deltaTime)
{
	if (timer->isOn()) if (checkPose()) {
		// Pose is correct
		LOG(INFO) << "Correct pose detected." << std::endl;
		
		changeCarState(RouteComponent::RouteState::Moving);
		timer->toggleTimer(false);
		scene->data.points++;
		SoundHandler::getInstance().playSoundSnippet("sounds/points_plus.wav");
	}
	else {
		timer->update(deltaTime);

		// Update overlay data
		scene->data.remainingTime = timer->getTimeRemaining();
		scene->data.progress = timer->getTimeRemaining() / timer->rolloverTime;
	}
}
