#pragma once

class GameObject;

class Component
{
protected:
	GameObject* gameObject;
public:
	Component();
	~Component();

	virtual void update(float elapsedTime) {};

	inline void setGameObject(GameObject* gameObject) { this->gameObject = gameObject; }
};

