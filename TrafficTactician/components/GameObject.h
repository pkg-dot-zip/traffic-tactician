#pragma once

#include <list>
#include <glm/glm.hpp>
#include <list>
#include <memory>
#include <optional>
#include <string>

class Simulation;
class Component;
class DrawComponent;
class BoundingBoxComponent;

class GameObject
{
	std::shared_ptr<DrawComponent> drawComponent;
	std::list<std::shared_ptr<Component>> components;

public:
	GameObject(const std::string &name, const std::weak_ptr<Simulation>& sim);
	~GameObject();

	std::string name;
	std::weak_ptr<Simulation> sim;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	void addComponent(const std::shared_ptr<Component>& component);
	void removeComponent(const std::shared_ptr<Component>& component);
	std::list<std::shared_ptr<Component>>& getComponents();
	void update(float deltaTime) const;
	void draw(const glm::mat4 & = glm::mat4(1.0f)) const;

	template <class T>
	std::optional<std::shared_ptr<T>> getComponent()
	{
		for (auto c : components)
		{
			if (std::shared_ptr<T> t = dynamic_pointer_cast<T>(c)) return t;
		}
		return std::nullopt;
	}

	template <class T>
	void removeComponent()
	{
		components.remove_if([](Component* c)
			{
				T* t = dynamic_cast<T*>(c);
				return t != nullptr;
			});
	}
};
