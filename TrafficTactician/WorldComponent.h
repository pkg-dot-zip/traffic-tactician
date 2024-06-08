#pragma once
#include <memory>
#include <vector>

#include "DrawComponent.h"
#include "ModelComponent.h"
#include "tigl.h"

class WorldComponent : public DrawComponent
{
	std::vector<std::shared_ptr<ModelComponent>> models;
	float gridSize;
	int worldSize;

	tigl::VBO* vbo;

public:
	WorldComponent(int worldSize, float gridSize, const std::shared_ptr<ModelComponent>& baseModel);
	~WorldComponent() = default;

	void setModel(int x, int y, const std::shared_ptr<ModelComponent>& model_component);

	void draw(glm::mat4 parentMatrix) override;
};
