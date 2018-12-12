#pragma once
#include <Rtti.h>

namespace blink
{
    class Entity;

    class Component
    {
    public:
        RTTI_ROOT(Component);

        friend class Entity;

    public:
        Component();
        virtual ~Component();

        virtual bool initialize(Entity* pEntity) = 0;
        virtual void terminate() = 0;

        Entity* getEntity() const { return m_entity; };

    private:
        Entity* m_entity{};

    };
}
