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
        static const float FIXED_FPS;
        static const float FIXED_DT;

    public:
        EcsWorld();

        bool initialize();
        void terminate();

        bool addSystem(ILogicalSystem* sys);

        template <typename T, typename = std::enable_if_t<std::is_base_of<ILogicalSystem, T>::value>> T* findSystem() const
        {
            auto it = m_logicalSystemsTypeMap.find(typeid(T));
            if (it == m_logicalSystemsTypeMap.end()) return nullptr;

            return dynamic_cast<T*>(it->second);
        }

        void destroyAllSystems();

        void step();

        flecs::world& getWorld() { return m_world; };
        [[nodiscard]] uint32_t getFrameTick() const { return m_frameTick; }

    private:
        flecs::world m_world;
        uint32_t m_frameTick{};
        std::vector<ILogicalSystem*> m_logicalSystems;
        std::unordered_map<std::type_index, ILogicalSystem*> m_logicalSystemsTypeMap;
    };
} // namespace blink
