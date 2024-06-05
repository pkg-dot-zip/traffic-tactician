#pragma once
#include "Component.h"
#include <string>
#include "InputHandler.h"

class ControllerComponent : public Component
{
	float poseTimer = 0;
public:
	ControllerComponent(Pose pose);
	~ControllerComponent() = default;
	Pose correctPose;
	bool checkPose();

	virtual void update(float deltaTime) override;
};

