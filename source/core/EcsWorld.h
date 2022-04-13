
/*********************************************************************
 * \file   EcsWorld.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/07/2022
 *********************************************************************/
#pragma once

#include <flecs/flecs.h>
#include <foundation/BaseTypes.h>

#include <typeindex>
#include <unordered_map>
#include <vector>

namespace blink
{
    class ILogicalSystem;

    class EcsWorld
    {
    public:
        EcsWorld();

        bool initialize();
        void terminate();

        bool addSystem(ILogicalSystem* sys);
        template <typename T> T* findSystem();
        void destroySystems();

        void step();

        flecs::world& getWorld() { return m_world; };

    private:
        flecs::world m_world;
        std::vector<ILogicalSystem*> m_logicalSystems;
        std::unordered_map<std::type_index, ILogicalSystem*> m_logicalSytemsTypeMap;
    };

    template <typename T> inline T* EcsWorld::findSystem()
    {
        auto it = m_logicalSytemsTypeMap.find(typeid(T));
        if (it == m_logicalSytemsTypeMap.end()) return nullptr;

        return dynamic_cast<T*>(it->second);
    }

} // namespace blink
