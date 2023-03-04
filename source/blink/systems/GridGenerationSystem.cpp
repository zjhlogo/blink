//
// Created by zjhlogo on 2023/3/4.
//
#include "GridGenerationSystem.h"
#include "../geometries_builder/LineListBuilder.h"
#include "blink/components/Components.h"
#include "core/components/Components.h"
#include "core/modules/IResourceModule.h"

namespace blink
{

    bool GridGenerationSystem::initialize()
    {
        m_lineListBuilder = new LineListBuilder();

        auto resourceModule = getResourceModule();
        m_gridMaterial = resourceModule->createMaterial("/materials/debug_line.mtl");
        m_gridGeometry = resourceModule->createGeometry(m_lineListBuilder->getUniqueId(), PrimitiveTopology::LineList);
        for (int i = -m_gridCount; i <= m_gridCount; ++i)
        {
            m_lineListBuilder->addLine(glm::vec3(i * m_gridGap, 0.0f, -m_gridCount * m_gridGap),
                                       glm::vec3(i * m_gridGap, 0.0f, m_gridCount * m_gridGap),
                                       Color::WHITE);
            m_lineListBuilder->addLine(glm::vec3(-m_gridCount * m_gridGap, 0.0f, i * m_gridGap),
                                       glm::vec3(m_gridCount * m_gridGap, 0.0f, i * m_gridGap),
                                       Color::WHITE);
        }

        m_lineListBuilder->build(m_gridGeometry);

        auto& world = m_ecsWorld->getWorld();

        // create grid entity
        m_gridEntity = world.entity("grid");
        m_gridEntity.set<Position>({glm::zero<glm::vec3>()});
        m_gridEntity.set<Rotation>({glm::identity<glm::quat>()});
        m_gridEntity.set<Scale>({glm::one<glm::vec3>()});
        m_gridEntity.set<Renderable>({RenderLayers::STATIC});
        m_gridEntity.set<StaticModel>({m_gridGeometry, m_gridMaterial});

        return true;
    }

    void GridGenerationSystem::terminate()
    {
        SAFE_RELEASE(m_gridGeometry);
        SAFE_RELEASE(m_gridMaterial);
        SAFE_DELETE(m_lineListBuilder);
    }
} // namespace blink
