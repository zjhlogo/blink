//
// Created by zjhlogo on 2023/3/4.
//
#pragma once

#include "core/systems/ILogicalSystem.h"

namespace blink
{
    class LineListBuilder;
    class IMaterial;
    class IGeometry;

    class GridGenerationSystem : public ILogicalSystem
    {
    public:
        GridGenerationSystem() = default;

        bool initialize() override;
        void terminate() override;

    private:
        flecs::entity m_gridEntity;

        LineListBuilder* m_lineListBuilder{};
        IMaterial* m_gridMaterial{};
        IGeometry* m_gridGeometry{};

        int m_gridCount{50};
        float m_gridGap{1.0f};
    };
} // namespace blink
