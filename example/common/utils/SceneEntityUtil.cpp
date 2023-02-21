/*********************************************************************
 * \file   SceneEntityUtil.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "SceneEntityUtil.h"
#include "../logical_systems/LockTargetCameraControllerSystem.h"

#include <blink/components/Components.h>
#include <core/components/Components.h>
#include <core/modules/IRenderModule.h>
#include <core/modules/IResourceModule.h>

bool SceneEntityUtil::initializeCommonLogicalSystems(blink::IApp* app)
{
    app->addLogicSystem(new LockTargetCameraControllerSystem());
    return true;
}

bool SceneEntityUtil::initializeCommonSceneEntities(blink::EcsWorld& ecsWorld)
{
    auto& world = ecsWorld.getWorld();
    auto resModule = blink::getResourceModule();
    auto renderModule = blink::getRenderModule();
    const auto& surfaceSize = renderModule->getSurfaceSize();

    // camera
    auto entityCamera = world.entity("main_camera")
                            .set<blink::Position>({glm::vec3(0.0f, 0.0f, 10.0f)})
                            .set<blink::Rotation>({glm::quatLookAt(glm::forward(), glm::up())})
                            .set<blink::CameraData>({glm::perspective(glm::radians(45.0f), surfaceSize.x / surfaceSize.y, 0.1f, 1000.0f)})
                            .set<LockTargetCameraData>({glm::zero<glm::vec3>(), glm::zero<glm::vec3>(), 10.0f, 0.01f});

    // light
    auto entityLight =
        world.entity("main_light").set<blink::Position>({glm::vec3(200.0f, 200.0f, -200.0f)}).set<blink::LightData>({glm::one<glm::vec3>(), 100.0f});

    world.entity().set<blink::RenderFeature>({blink::RenderOrders::DYNAMIC_OPAQUE, blink::RenderLayers::ALL, nullptr});

    return true;
}
