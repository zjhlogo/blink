
/*********************************************************************
 * \file   EcsWorld.cpp
 * \brief
 *
 * \author
 * zjhlogo
 * \date   04/07/2022
 *********************************************************************/
#include "EcsWorld.h"
#include "base/ILogicalSystem.h"

namespace blink
{

    bool EcsWorld::initialize()
    {
        for (auto sys : m_logicalSystems)
        {
            if (!sys->initialize()) return false;
        }

        return true;
    }

    void EcsWorld::terminate()
    {
        for (int i = (int)m_logicalSystems.size() - 1; i >= 0; --i)
        {
            m_logicalSystems[i]->terminate();
        }
    }

    bool EcsWorld::addSystem(ILogicalSystem* sys)
    {
        std::type_index typeIndex = typeid(*sys);
        m_logicalSytemsTypeMap.emplace(typeIndex, sys);
        m_logicalSystems.push_back(sys);

        sys->setEcsWorld(this);

        return true;
    }

    void EcsWorld::destroySystems()
    {
        for (int i = (int)m_logicalSystems.size() - 1; i >= 0; --i)
        {
            SAFE_DELETE(m_logicalSystems[i]);
        }

        m_logicalSystems.clear();
    }

    void EcsWorld::step()
    {
        for (auto sys : m_logicalSystems)
        {
            sys->framePreUpdate();
        }

        //
        m_world.progress();

        for (auto sys : m_logicalSystems)
        {
            sys->framePostUpdate();
        }
    }
} // namespace blink
