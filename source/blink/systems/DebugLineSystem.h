/*********************************************************************
 * \file   DebugLineSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include <core/systems/ILogicalSystem.h>
#include <core/types/Color.h>

namespace blink
{
    class LineListBuilder;
    class IMaterial;
    class IGeometry;

    class DebugLineSystem : public ILogicalSystem
    {
    public:
        DebugLineSystem() = default;

        bool initialize() override;
        void terminate() override;
        void framePostUpdate() override;

        void addLine(const glm::vec3& p1, const glm::vec3& p2, const Color& color);

    private:
        flecs::entity m_debugEntity;

        LineListBuilder* m_lineListBuilder{};
        IMaterial* m_debugMaterial{};
        IGeometry* m_debugGeometry{};
    };
} // namespace blink
