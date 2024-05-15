#pragma once

#include "Component.h"

class SpinComponent : public Component
{
	float speed;

public:
	SpinComponent(float speed = 1.0f);
	~SpinComponent();

	virtual void update(float deltaTime) override;
};
