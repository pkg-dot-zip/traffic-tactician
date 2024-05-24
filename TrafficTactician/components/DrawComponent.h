#pragma once

#include <glm/fwd.hpp>
#include <glm/matrix.hpp>

#include "Component.h"

class DrawComponent : public Component
{
public:
	DrawComponent();
	~DrawComponent();
	virtual void draw(glm::mat4 parentMatrix = glm::mat4(1.0f)) = 0;
};
