#include "WorldComponent.h"

#include <glm/ext/matrix_transform.hpp>

#include "easylogging++.h"
#include "GameObject.h"

WorldComponent::WorldComponent(int worldSize, float gridSize, const std::shared_ptr<ModelComponent>& baseModel) : gridSize(gridSize), worldSize(worldSize)
{
	models.resize(worldSize * worldSize);
	models.clear();

	for (int i = 0; i < worldSize * worldSize; i++)
	{
		models.push_back(baseModel);
	}
}

void WorldComponent::setModel(int x, int y, const std::shared_ptr<ModelComponent>& model_component)
{
	models[x * worldSize + y] = model_component;
}

void WorldComponent::draw(glm::mat4 parentMatrix)
{
	for (int i = 0; i < models.size(); i++)
	{
		glm::mat4 modelMatrix = glm::translate(parentMatrix, glm::vec3(i / worldSize * gridSize, 0, i % worldSize * gridSize)); // Set model in correct position in grid
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-worldSize / 2.0f + gridSize / 2, 0.0f, -worldSize / 2.0f + gridSize / 2)); // Move entire grid

		if (models[i] != nullptr)
		{
			models[i]->draw(modelMatrix);
		}
		else
		{
			LOG(WARNING) << "NO MODEL FOUND" << std::endl;
		}
	}
}
