
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
    static int my_run_action(ecs_world_t* world, ecs_app_desc_t* desc)
    {
        if (desc->init)
        {
            desc->init(world);
        }

        return 0;
    }

    EcsWorld::EcsWorld()
    {
        ecs_app_set_run_action(my_run_action);
        m_world.app().threads(4).target_fps(60.0f).enable_rest().run();
    }

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
