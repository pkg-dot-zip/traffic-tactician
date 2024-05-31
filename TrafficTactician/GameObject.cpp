#include "GameObject.h"
#include "Simulation.h"
#include "DrawComponent.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "BoundingBoxComponent.h"


GameObject::GameObject(const std::string& name, Simulation* sim)
{
	this->name = name;
	this->sim = sim;
}

GameObject::~GameObject() = default;

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	component->setGameObject(this);
	components.push_back(component);

	if (drawComponent == nullptr)
	{
		drawComponent = dynamic_pointer_cast<DrawComponent>(component);
	}
}

void GameObject::removeComponent(std::shared_ptr<Component> component)
{
	components.remove(component);
}

std::list<std::shared_ptr<Component>> GameObject::getComponents()
{
	return components;
}

void GameObject::draw(const glm::mat4& parentMatrix)
{
	if (!drawComponent) return;

	glm::mat4 modelMatrix = parentMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	drawComponent->draw(modelMatrix);
}

void GameObject::update(float deltaTime)
{
	for (auto& c : components)
	{
		c->update(deltaTime);
	}
}
