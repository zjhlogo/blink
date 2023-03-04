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
#include <blink/systems/GridGenerationSystem.h>
#include <blink/utils/BuiltinResource.h>
#include <core/components/Components.h>
#include <core/modules/IRenderModule.h>
#include <core/modules/IResourceModule.h>

bool SceneEntityUtil::initializeCommonLogicalSystems(blink::IApp* app)
{
    app->addLogicSystem(new blink::GridGenerationSystem());
    app->addLogicSystem(new LockTargetCameraControllerSystem());
    return true;
}

bool SceneEntityUtil::initializeCommonSceneEntities(blink::EcsWorld& ecsWorld)
{
    auto& world = ecsWorld.getWorld();
    auto renderModule = blink::getRenderModule();
    const auto& surfaceSize = renderModule->getSurfaceSize();

    // camera
    auto entityCamera = world.entity("main_camera");
    entityCamera.set<blink::Position>({glm::vec3(0.0f, 0.0f, 10.0f)});
    entityCamera.set<blink::Rotation>({glm::quatLookAt(glm::forward(), glm::up())});
    entityCamera.set<blink::Scale>({glm::one<glm::vec3>()});
    entityCamera.set<blink::CameraData>({glm::perspective(glm::radians(45.0f), surfaceSize.x / surfaceSize.y, 0.1f, 1000.0f)});
    entityCamera.set<LockTargetCameraData>({{glm::zero<glm::vec3>()}, glm::zero<glm::vec3>(), 10.0f, 0.01f});

    // light
    auto entityLight = world.entity("main_light");
    entityLight.set<blink::Position>({glm::vec3(1.0f, 1.0f, -1.0f)});
    entityLight.set<blink::Rotation>({glm::identity<glm::quat>()});
    entityLight.set<blink::Scale>({glm::vec3(0.2f, 0.2f, 0.2f)});
    entityLight.set<blink::LightData>({glm::one<glm::vec3>(), 100.0f});
    //    entityLight.set<blink::DebugDrawing>({1.0f});
    entityLight.set<blink::StaticModel>({blink::BuiltinResource::getSphereGeometry(), blink::BuiltinResource::getUnlitMaterial()});
    entityLight.set<blink::Renderable>({blink::RenderLayers::STATIC});

    // add default render feature
    auto entityRenderFeature = world.entity();
    entityRenderFeature.set<blink::RenderFeature>({blink::RenderOrders::DYNAMIC_OPAQUE, blink::RenderLayers::ALL, nullptr});

    return true;
}
