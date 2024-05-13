#pragma once

#include "Component.h"

class SpinComponent : public Component
{
	float speed;
public:
	SpinComponent(float speed);
	~SpinComponent();

	virtual void update(float elapsedTime) override;
};

