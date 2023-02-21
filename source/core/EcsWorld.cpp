/*********************************************************************
 * \file   EcsWorld.cpp
 * \brief
 *
 * \author
 * zjhlogo
 * \date   04/07/2022
 *********************************************************************/
#include "EcsWorld.h"
#include "systems/ILogicalSystem.h"

#include <flecs/flecs_os_api_stdcpp.h>

namespace blink
{
    constexpr float EcsWorld::FIXED_FPS = 60.0f;
    constexpr float EcsWorld::FIXED_DT = 1.0f / EcsWorld::FIXED_FPS;

    static int myRunAction(ecs_world_t* world, ecs_app_desc_t* desc)
    {
        if (desc->init)
        {
            desc->init(world);
        }

        return 0;
    }

    EcsWorld::EcsWorld()
    {
        stdcpp_set_os_api();
        ecs_app_set_run_action(myRunAction);
        m_world.app().threads(4).target_fps(FIXED_FPS).enable_rest().run();
    }

    bool EcsWorld::initialize()
    {
        for (auto sys : m_logicalSystems)
        {
            if (!sys->initialize())
            {
                return false;
            }
        }

        return true;
    }

    void EcsWorld::terminate()
    {
        //
        for (int i = static_cast<int>(m_logicalSystems.size()) - 1; i >= 0; --i)
        {
            //
            m_logicalSystems[i]->terminate();
        }
    }

    bool EcsWorld::addSystem(ILogicalSystem* sys)
    {
        std::type_index typeIndex = typeid(*sys);
        m_logicalSystemsTypeMap.emplace(typeIndex, sys);
        m_logicalSystems.push_back(sys);

        sys->setEcsWorld(this);

        return true;
    }

    void EcsWorld::destroyAllSystems()
    {
        for (int i = static_cast<int>(m_logicalSystems.size()) - 1; i >= 0; --i)
        {
            //
            SAFE_DELETE_AND_TERMINATE(m_logicalSystems[i]);
        }

        m_logicalSystems.clear();
    }

    void EcsWorld::step()
    {
        // LOGI("frame tick: {0}", m_frameTick);

        for (auto sys : m_logicalSystems)
        {
            sys->framePreUpdate();
        }

        //
        m_world.progress(FIXED_DT);

        for (auto sys : m_logicalSystems)
        {
            sys->framePostUpdate();
        }

        ++m_frameTick;
    }
} // namespace blink
