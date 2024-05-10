#pragma once
#include <glm/glm.hpp>
#include "Component.h"
class GameObject;

class BoundingBoxComponent : public Component
{
	glm::vec3 min;
	glm::vec3 max;
public:
	bool collides(GameObject* other);
};

