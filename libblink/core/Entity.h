#pragma once

#include <Rtti.h>
#include <vector>

namespace blink
{
    class Component;

    class Entity
    {
    public:
        RTTI_ROOT(Entity);

        typedef std::vector<Component*> ComponentVector;

    public:
        bool insertComponent(Component* component, int index = -1);
        bool removeComponent(Component* component);
        void removeAllComponents();

        template <typename T> T* findComponent()
        {
            std::type_index baseTypeIndex = typeid(T);

            for (auto component : m_componentList)
            {
                if (component->getRtti()->isDerivedFrom(baseTypeIndex)) return (T*)component;
            }

            return nullptr;
        }

    protected:
        Entity();
        virtual ~Entity();

    private:
        ComponentVector m_componentList;

    };
}
