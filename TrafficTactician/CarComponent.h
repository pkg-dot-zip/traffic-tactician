#pragma once

#include "Component.h"
#include "DrawComponent.h" // showing car route points for debugging
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "tigl.h"
using tigl::Vertex;

class CarComponent : public DrawComponent
{
	double lastSpawn = 0;
	double clickDelay = 4;
public:
	CarComponent();
	~CarComponent();

	std::vector<glm::vec3> points;
	std::vector<Vertex> verts;

	bool allowPointClicks = false;

	virtual void update(float elapsedTime) override;
	virtual void draw(glm::mat4 parentMatrix) override;
};

