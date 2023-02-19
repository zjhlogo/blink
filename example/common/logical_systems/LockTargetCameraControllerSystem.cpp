/*********************************************************************
 * \file   LockTargetCameraControllerSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "LockTargetCameraControllerSystem.h"
#include "../utils/ImguiExtension.h"

#include <imgui/imgui.h>

bool LockTargetCameraControllerSystem::initialize()
{
    auto& world = m_ecsWorld->getWorld();
    m_cameraQuery = world.query<blink::Position, blink::Rotation, LockTargetCameraData>();

    return true;
}

void LockTargetCameraControllerSystem::terminate()
{
}

void LockTargetCameraControllerSystem::framePreUpdate()
{
}

void LockTargetCameraControllerSystem::framePostUpdate()
{
    glm::vec2 leftMouseDelta{};
    float wheelDelta{};
    if (!updateLeftMouseDelta(leftMouseDelta) && !updateMouseWheelDelta(wheelDelta)) return;

    m_cameraQuery.each(
        [&](blink::Position& pos, blink::Rotation& rot, LockTargetCameraData& cameraData)
        {
            cameraData.eulerRotation.y -= leftMouseDelta.x * cameraData.rotateSensitive;
            cameraData.eulerRotation.x -= leftMouseDelta.y * cameraData.rotateSensitive;
            cameraData.cameraDistance -= wheelDelta;

            // calculate the new camera position and rotation
            rot.value = glm::quat(cameraData.eulerRotation);
            pos.value = glm::rotate(rot.value, glm::vec3(0.0f, 0.0f, cameraData.cameraDistance));
        });
}

bool LockTargetCameraControllerSystem::updateLeftMouseDelta(glm::vec2& mouseDeltaOut)
{
    auto& io = ImGui::GetIO();
    auto isMouseDown = io.MouseDown[0];
    if (io.WantCaptureMouse || !isMouseDown) return false;

    mouseDeltaOut = {io.MouseDelta.x, io.MouseDelta.y};

    return true;
}

bool LockTargetCameraControllerSystem::updateMouseWheelDelta(float& wheelOut)
{
    auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse || (ImGuiExt::lastFrameMouseWheel > -0.1f && ImGuiExt::lastFrameMouseWheel < 0.1f)) return false;

    wheelOut = ImGuiExt::lastFrameMouseWheel;
    if (io.KeyShift) wheelOut *= 0.1f;

    return true;
}
