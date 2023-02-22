/*********************************************************************
 * \file   DebugLineSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#include "DebugLineSystem.h"

#include <blink/components/Components.h>
#include <core/EcsWorld.h>
#include <core/components/Components.h>
#include <core/modules/IResourceModule.h>

namespace blink
{
    bool DebugLineSystem::initialize()
    {
        auto resourceModule = getResourceModule();
        m_debugMaterial = resourceModule->createMaterial("/materials/debug_line.mtl");
        m_debugGeometry = resourceModule->createGeometry(m_lineListBuilder.getUniqueId(), PrimitiveTopology::LineList);

        auto& world = m_ecsWorld->getWorld();

        // create debug entity
        m_debugEntity = world.entity("debug");
        m_debugEntity.set<Position>({glm::zero<glm::vec3>()});
        m_debugEntity.set<Rotation>({glm::identity<glm::quat>()});
        m_debugEntity.set<Renderable>({RenderLayers::DEBUG});
        m_debugEntity.set<StaticModel>({m_debugGeometry, m_debugMaterial});
        m_debugEntity.set<RenderFeature>({RenderOrders::STATIC_OPAQUE, RenderLayers::DEBUG, m_debugMaterial});

        return true;
    }

    void DebugLineSystem::terminate()
    {
        SAFE_RELEASE(m_debugGeometry);
        SAFE_RELEASE(m_debugMaterial);
    }

    void DebugLineSystem::framePostUpdate()
    {
        auto& world = m_ecsWorld->getWorld();

        {
            static auto debugDrawingQuery = world.query_builder<const Position, const Rotation, const DebugDrawing>().build();
            debugDrawingQuery.each([&](flecs::entity e, const blink::Position& pos, const blink::Rotation& rot, const DebugDrawing& debug) {
                auto up = glm::rotate(rot.value, glm::up());
                auto forward = glm::rotate(rot.value, glm::forward());
                auto right = glm::rotate(rot.value, glm::right());

                m_lineListBuilder.addLine(pos.value, pos.value + up * debug.lineLength, Color::GREEN);
                m_lineListBuilder.addLine(pos.value, pos.value + forward * debug.lineLength, Color::BLUE);
                m_lineListBuilder.addLine(pos.value, pos.value + right * debug.lineLength, Color::RED);
            });
        }

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
