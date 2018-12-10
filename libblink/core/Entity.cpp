#include "Entity.h"
#include "Component.h"
#include <algorithm>

bool Entity::insertComponent(Component * component, int index/* = -1 */)
{
	if (index < 0 || index > static_cast<int>(m_componentList.size()))
	{
		index = static_cast<int>(m_componentList.size());
	}

	m_componentList.insert(m_componentList.begin() + index, component);
	component->m_entity = this;
	// TODO: initialize component

	return true;
}

bool Entity::removeComponent(Component * component)
{
	auto it = std::find(m_componentList.begin(), m_componentList.end(), component);
	if (it != m_componentList.end())
	{
		m_componentList.erase(it);
		// TODO: delete component
		return true;
	}

	return false;
}
