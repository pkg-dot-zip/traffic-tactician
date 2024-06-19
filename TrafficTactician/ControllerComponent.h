#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include "CameraInputHandler.h"
#include "Scene.h"
#include "Timer.h"
#include "RouteComponent.h"


class ControllerComponent : public Component
{
	Scene* scene;
public:
	std::shared_ptr<Timer> timer;
	Pose correctPose;

	ControllerComponent(Pose pose, Scene* scene);
	~ControllerComponent() = default;

	bool checkPose() const;
	void changeCarState(RouteComponent::RouteState state) const;
	void timerCallback() const;

	virtual void update(float deltaTime) override;
};

