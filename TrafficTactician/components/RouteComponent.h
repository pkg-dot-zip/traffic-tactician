#pragma once

#include "Component.h"
#include <vector>
#include "glm/ext/matrix_transform.hpp"
using glm::vec3;

class RouteComponent : public Component
{
	std::vector<vec3> routeNodes;
	float direction;
	float speed;
public:
	RouteComponent(float speed);
	~RouteComponent();

	virtual void update(float elapsedTime) override;
};
