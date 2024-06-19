#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class MoveComponent : public Component
{
public:
	glm::vec3 direction;
	MoveComponent(const glm::vec3& direction);
	~MoveComponent();

	void nextDirection(const glm::vec3& direction);
	virtual void update(float elapsedTime) override;
};

