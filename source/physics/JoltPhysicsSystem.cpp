//
// Created by zjhlogo on 2023/4/21.
//
#include "JoltPhysicsSystem.h"
#include "core/components/Components.h"

#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>
#include <foundation/Log.h>

#include <thread>

// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char* inFMT, ...)
{
    // Format the message
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);

    // Print to the TTY
    LOGI(buffer);
}

#ifdef JPH_ENABLE_ASSERTS

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
{
    // Print to the TTY
    LOGI("{0}:{1}: ({2}) {3}", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));

    // Breakpoint
    return true;
};

#endif // JPH_ENABLE_ASSERTS

namespace Layers
{
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
}; // namespace Layers

namespace BroadPhaseLayers
{
    static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
    static constexpr JPH::BroadPhaseLayer MOVING(1);
    static constexpr JPH::uint NUM_LAYERS(2);
}; // namespace BroadPhaseLayers

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
    BPLayerInterfaceImpl()
    {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
    }

    virtual JPH::uint GetNumBroadPhaseLayers() const override { return BroadPhaseLayers::NUM_LAYERS; }

    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
    {
        using namespace JPH;
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
    {
        switch ((JPH::BroadPhaseLayer::Type)inLayer)
        {
        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
            return "NON_MOVING";
        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
            return "MOVING";
        default:
            using namespace JPH;
            JPH_ASSERT(false);
            return "INVALID";
        }
    }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
    virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
    {
        switch (inLayer1)
        {
        case Layers::NON_MOVING:
            return inLayer2 == BroadPhaseLayers::MOVING;
        case Layers::MOVING:
            return true;
        default:
            using namespace JPH;
            JPH_ASSERT(false);
            return false;
        }
    }
};

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
    virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
    {
        switch (inObject1)
        {
        case Layers::NON_MOVING:
            return inObject2 == Layers::MOVING; // Non moving only collides with moving
        case Layers::MOVING:
            return true; // Moving collides with everything
        default:
            using namespace JPH;
            JPH_ASSERT(false);
            return false;
        }
    }
};

namespace blink
{
    JoltPhysicsSystem::JoltPhysicsSystem(bool pauseOnStartup)
        : m_pause(pauseOnStartup)
    {
    }

    bool JoltPhysicsSystem::initialize()
    {
        // Register allocation hook
        JPH::RegisterDefaultAllocator();

        // Install callbacks
        JPH::Trace = TraceImpl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

        // Create a factory
        JPH::Factory::sInstance = new JPH::Factory();

        // Register all Jolt physics types
        JPH::RegisterTypes();

        // Now we can create the actual physics system.
        static BPLayerInterfaceImpl broad_phase_layer_interface;
        static ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
        static ObjectLayerPairFilterImpl object_vs_object_layer_filter;

        m_physicsSystem = new JPH::PhysicsSystem();
        m_physicsSystem->Init(65536,
                              0,
                              65536,
                              10240,
                              broad_phase_layer_interface,
                              object_vs_broadphase_layer_filter,
                              object_vs_object_layer_filter);

        // sync rigid body pos and rot
        JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();
        std::uint32_t index = 0;
        m_ecsWorld->getWorld().system<Position, Rotation, const PhysicsData>().each(
            [&](flecs::entity e, Position& pos, Rotation& rot, const PhysicsData& pd) {
                JPH::BodyID bodyId(pd.bodyId);

                JPH::Vec3 phPos;
                JPH::Quat phRot;
                bodyInterface.GetPositionAndRotation(bodyId, phPos, phRot);

                pos.value = glm::vec3(phPos.GetX(), phPos.GetY(), phPos.GetZ());
                rot.value = glm::quat(phRot.GetX(), phRot.GetY(), phRot.GetZ(), phRot.GetW());

                // dump frame hash
                m_frameHash ^= index++;

                m_frameHash ^= *(std::uint32_t*)&pos.value.x;
                m_frameHash ^= *(std::uint32_t*)&pos.value.y;
                m_frameHash ^= *(std::uint32_t*)&pos.value.z;

                m_frameHash ^= *(std::uint32_t*)&rot.value.x;
                m_frameHash ^= *(std::uint32_t*)&rot.value.y;
                m_frameHash ^= *(std::uint32_t*)&rot.value.z;
                m_frameHash ^= *(std::uint32_t*)&rot.value.w;
            });

        return true;
    }

    void JoltPhysicsSystem::terminate()
    {
        SAFE_DELETE(m_physicsSystem);

        // Unregisters all types with the factory and cleans up the default material
        JPH::UnregisterTypes();

        // Destroy the factory
        SAFE_DELETE(JPH::Factory::sInstance);
    }

    void JoltPhysicsSystem::frameUpdate(float dt)
    {
        static JPH::TempAllocatorImpl temp_allocator(10 * 1024 * 1024);
        static JPH::JobSystemThreadPool job_system(2048, 8, std::thread::hardware_concurrency() - 1);

        if (m_needOptimizeBroadPhase)
        {
            m_needOptimizeBroadPhase = false;
            m_physicsSystem->OptimizeBroadPhase();
        }

        m_frameHash = 0;

        if (!m_pause)
        {
            ++m_frameTick;
            m_physicsSystem->Update(dt, 1, 1, &temp_allocator, &job_system);
        }

        if (m_pauseFrameTick == m_frameTick)
        {
            m_pause = true;
        }
    }

    std::uint32_t JoltPhysicsSystem::CreateBox(const glm::vec3& size,
                                               const glm::vec3& pos,
                                               const glm::quat& rot,
                                               PhysicsBodyType type)
    {
        JPH::ObjectLayer layer = Layers::NON_MOVING;
        if (type != PhysicsBodyType::Static)
        {
            layer = Layers::MOVING;
        }

        JPH::BodyCreationSettings boxSetting(new JPH::BoxShape(JPH::Vec3(size.x, size.y, size.z) * 0.5f),
                                             JPH::Vec3(pos.x, pos.y, pos.z),
                                             JPH::Quat(rot.x, rot.y, rot.z, rot.w),
                                             (JPH::EMotionType)type,
                                             layer);

        JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();
        JPH::BodyID boxId = bodyInterface.CreateAndAddBody(boxSetting, JPH::EActivation::Activate);

        m_needOptimizeBroadPhase = true;
        return boxId.GetIndexAndSequenceNumber();
    }

    std::uint32_t
    JoltPhysicsSystem::CreateSphere(float radius, const glm::vec3& pos, const glm::quat& rot, PhysicsBodyType type)
    {
        JPH::ObjectLayer layer = Layers::NON_MOVING;
        if (type != PhysicsBodyType::Static)
        {
            layer = Layers::MOVING;
        }

        JPH::BodyCreationSettings sphereSetting(new JPH::SphereShape(radius),
                                                JPH::Vec3(pos.x, pos.y, pos.z),
                                                JPH::Quat(rot.x, rot.y, rot.z, rot.w),
                                                (JPH::EMotionType)type,
                                                layer);
        sphereSetting.mRestitution = 0.5f;

        JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();
        JPH::BodyID sphereId = bodyInterface.CreateAndAddBody(sphereSetting, JPH::EActivation::Activate);

        m_needOptimizeBroadPhase = true;
        return sphereId.GetIndexAndSequenceNumber();
    }

    void JoltPhysicsSystem::DestroyBody(std::uint32_t bodyId)
    {
        JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();

        JPH::BodyID phBodyId(bodyId);
        bodyInterface.RemoveBody(phBodyId);
        bodyInterface.DestroyBody(phBodyId);
    }
} // namespace blink
