#include "Entity.h"
#include "Component.h"
#include <algorithm>

namespace blink
{
    bool Entity::insertComponent(Component * component, int index/* = -1 */)
    {
        if (index < 0 || index > static_cast<int>(m_componentList.size()))
        {
            index = static_cast<int>(m_componentList.size());
        }

        m_componentList.insert(m_componentList.begin() + index, component);
        component->m_entity = this;

        // initialize component
        if (!component->initialize(this))
        {
            SAFE_DELETE_AND_TERMINATE(component);
            return false;
        }

        return true;
    }

    bool Entity::removeComponent(Component * component)
    {
        auto it = std::find(m_componentList.begin(), m_componentList.end(), component);
        if (it != m_componentList.end())
        {
            m_componentList.erase(it);

            // terminate component
            SAFE_DELETE_AND_TERMINATE(component);

            return true;
        }

        return false;
    }

    void Entity::removeAllComponents()
    {
        for (auto component : m_componentList)
        {
            // terminate component
            SAFE_DELETE_AND_TERMINATE(component);
        }

        m_componentList.clear();
    }

    Entity::Entity()
    {

    }

    Entity::~Entity()
    {
        removeAllComponents();
    }
}
