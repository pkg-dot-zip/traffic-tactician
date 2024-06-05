#include "ControllerComponent.h"
#include "GameObject.h"
#include "RouteComponent.h"
#include "InputHandler.h"
#include <iostream>

ControllerComponent::ControllerComponent(Pose pose)
{
	this->correctPose = pose;
}


bool ControllerComponent::checkPose()
{
	// get the pose of the person
	Pose pose = getInputPose();
	// if the pose is the same as the correct pose, return true
	if (pose == correctPose)
	{
		return true;
	}
	return false;
}

void ControllerComponent::update(float deltaTime)
{
	//std::cout << "Pose : " << getPoseString(correctPose) << std::endl;

	// create a timer for 5 seconds
	poseTimer += deltaTime;
	if (poseTimer >= 5)
	{
		// if the timer is greater than 5 seconds, reset the timer 
		poseTimer = 0;
		if (!checkPose()) return;

		gameObject->getComponent<RouteComponent>()->crossed = true;
		gameObject->getComponent<RouteComponent>()->state = RouteComponent::RouteState::Moving;
	}

	
}
