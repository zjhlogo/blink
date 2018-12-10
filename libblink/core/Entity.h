#pragma once

#include <vector>

class Component;

class Entity
{
public:
	typedef std::vector<Component*> ComponentVector;

public:
	bool insertComponent(Component* component, int index = -1);
	bool removeComponent(Component* component);

private:
	ComponentVector m_componentList;

};
