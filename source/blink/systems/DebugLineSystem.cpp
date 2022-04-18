
/*********************************************************************
 * \file   DebugLineSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#include "DebugLineSystem.h"
#include "../geometries/LineListGeometry.h"
#include "../materials/Material.h"
#include "../resources/ResourceMgr.h"

#include <blink/components/Components.h>
#include <core/EcsWorld.h>
#include <core/components/Components.h>

namespace blink
{
    bool DebugLineSystem::initialize()
    {
        m_debugMaterial = ResourceMgr::getInstance().createMaterial("resource/materials/wireframe_linelist.mtl");
        m_debugGeometry = ResourceMgr::getInstance().createGeometry<LineListGeometry>(m_lineListBuilder.getUniqueId());

        auto& world = m_ecsWorld->getWorld();

        // create debug entity
        m_debug = world.entity("debug");
        m_debug.set<Position>({glm::zero<glm::vec3>()});
        m_debug.set<Rotation>({glm::identity<glm::quat>()});
        m_debug.set<Renderable>({RenderLayers::DEBUG});
        m_debug.set<StaticModel>({m_debugGeometry, m_debugMaterial});
        m_debug.set<RenderFeature>({RenderOrders::STATIC_OPAQUE, RenderLayers::DEBUG, m_debugMaterial});

        return true;
    }

    void DebugLineSystem::terminate()
    {
        SAFE_RELEASE(m_debugGeometry);
        SAFE_RELEASE(m_debugMaterial);
    }

    void DebugLineSystem::framePostUpdate()
    {
        // flush debug geometry
        m_lineListBuilder.build(m_debugGeometry);

        // reset debug lines
        m_lineListBuilder.reset();
    }

    void DebugLineSystem::addLine(const glm::vec3& p1, const glm::vec3& p2, const Color& color)
    {
        //
        m_lineListBuilder.addLine(p1, p2, color);
    }
} // namespace blink
