//
// Created by zjhlogo on 2023/4/21.
//
#pragma once

#include "components/Components.h"
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

        std::uint32_t CreateBox(const glm::vec3& size, const glm::vec3& pos, const glm::quat& rot, PhysicsBodyType type);
        std::uint32_t CreateSphere(float radius, const glm::vec3& pos, const glm::quat& rot, PhysicsBodyType type);

        void DestroyBody(std::uint32_t bodyId);

    private:
        JPH::PhysicsSystem* m_physicsSystem{};
        bool m_needOptimizeBroadPhase{};

    };
}
