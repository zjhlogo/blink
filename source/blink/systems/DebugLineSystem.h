
/*********************************************************************
 * \file   DebugLineSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include "../geometries_builder/LineListBuilder.h"

#include <core/ILogicalSystem.h>
#include <core/resources/IGeometry.h>
#include <core/resources/IMaterial.h>
#include <core/types/Color.h>

namespace blink
{
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
        IMaterial* m_debugMaterial{};
        IGeometry* m_debugGeometry{};
    };

} // namespace blink
