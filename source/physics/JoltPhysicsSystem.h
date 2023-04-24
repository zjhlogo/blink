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
        JoltPhysicsSystem(bool pauseOnStartup = false);

        bool initialize() override;
        void terminate() override;

        void frameUpdate(float dt) override;

        std::uint32_t
        CreateBox(const glm::vec3& size, const glm::vec3& pos, const glm::quat& rot, PhysicsBodyType type);
        std::uint32_t CreateSphere(float radius, const glm::vec3& pos, const glm::quat& rot, PhysicsBodyType type);

        void DestroyBody(std::uint32_t bodyId);

        const std::uint32_t getFrameTick() const { return m_frameTick; }
        const std::uint32_t getFrameHash() const { return m_frameHash; }

        void setPause(bool pause) { m_pause = pause; }
        bool isPause() const { return m_pause; }

        void setPauseFrameTick(std::uint32_t tick) { m_pauseFrameTick = tick; }

        void saveState(const tstring& filePath);
        void loadState(const tstring& filePath);

    private:
        JPH::PhysicsSystem* m_physicsSystem{};
        bool m_needOptimizeBroadPhase{};
        bool m_pause{};

        std::uint32_t m_frameTick{};
        std::uint32_t m_frameHash{};
        std::uint32_t m_pauseFrameTick{};
    };
} // namespace blink
