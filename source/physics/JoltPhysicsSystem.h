//
// Created by zjhlogo on 2023/4/21.
//
#pragma once

#include <core/systems/ILogicalSystem.h>

namespace JPH
{
    class PhysicsSystem;
}

namespace blink
{
    class JoltPhysicsSystem final : public ILogicalSystem
    {
    public:
        bool initialize() override;
        void terminate() override;

        void frameUpdate(float dt) override;

    private:
        JPH::PhysicsSystem* m_physicsSystem{};

    };
}
