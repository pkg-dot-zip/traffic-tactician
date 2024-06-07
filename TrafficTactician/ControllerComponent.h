#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include "InputHandler.h"
#include "Scene.h"
#include "Timer.h"

class ControllerComponent : public Component
{
	Scene* scene;
public:
	std::shared_ptr<Timer> timer;
	Pose correctPose;

	ControllerComponent(Pose pose, Scene* scene);
	~ControllerComponent() = default;

	bool checkPose();
	void timerCallback();

	virtual void update(float deltaTime) override;
};

