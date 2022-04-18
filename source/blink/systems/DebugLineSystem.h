
/*********************************************************************
 * \file   DebugLineSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include "../geometries_builder/LineListBuilder.h"

#include <core/base/Color.h>
#include <core/base/ILogicalSystem.h>

namespace blink
{
    class Material;
    class LineListGeometry;

    class DebugLineSystem : public ILogicalSystem
    {
    public:
        DebugLineSystem() = default;

        virtual bool initialize() override;
        virtual void terminate() override;
        virtual void framePostUpdate() override;

        void addLine(const glm::vec3& p1, const glm::vec3& p2, const Color& color);

    private:
        flecs::entity m_debug;

        LineListBuilder m_lineListBuilder;
        Material* m_debugMaterial{};
        LineListGeometry* m_debugGeometry{};
    };

} // namespace blink
