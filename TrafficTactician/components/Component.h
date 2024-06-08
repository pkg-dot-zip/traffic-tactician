#pragma once

class GameObject;

class Component
{
public:
	Component() = default;
	~Component() = default;

	virtual void update(float deltaTime)
	{
	};

	inline void setGameObject(GameObject* gameObject) { this->gameObject = gameObject; }
protected:
	GameObject* gameObject;
};
