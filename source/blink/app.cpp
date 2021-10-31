/**

    @file      app.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      31.10.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#include "app.h"

#include <blink/base/ISystemBase.h>
#include <blink/component/Components.h>
#include <blink/util/RenderUtil.cpp>

NS_BEGIN

IApp::~IApp()
{
    destroyAllSystems();
}

bool IApp::postInitialize()
{
    for (auto sys : m_systems)
    {
        if (!sys->initialize(m_world)) return false;
    }

    return true;
}

void IApp::preTerminate()
{
    for (int i = (int)m_systems.size() - 1; i >= 0; --i)
    {
        m_systems[i]->terminate(m_world);
    }
}

void IApp::update(float dt)
{
    m_world.progress(dt);
}

void IApp::render(VulkanCommandBuffer& commandBuffer)
{
    m_world.each([&](flecs::entity e, const NS::Position& pos, const NS::Rotation& rot, const NS::StaticModel& model)
                 { NS::RenderUtil::drawMesh(commandBuffer, model.mesh, model.material, pos.value, rot.value); });
}

bool IApp::addSystem(ISystemBase* sys)
{
    m_systems.push_back(sys);

    return false;
}

void IApp::destroyAllSystems()
{
    for (int i = (int)m_systems.size() - 1; i >= 0; --i)
    {
        SAFE_DELETE(m_systems[i]);
    }

    m_systems.clear();
}

NS_END
