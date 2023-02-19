/*********************************************************************
 * \file   LockTargetCameraControllerSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once
#include "../common_components/LockTargetCameraData.h"

#include <blink/components/Components.h>
#include <core/ILogicalSystem.h>
#include <core/components/Components.h>

class LockTargetCameraControllerSystem : public blink::ILogicalSystem
{
public:
    bool initialize() override;
    void terminate() override;

    void framePreUpdate() override;
    void framePostUpdate() override;

private:
    flecs::query<blink::Position, blink::Rotation, blink::CameraData> m_cameraQuery;

    bool m_prevMouseDown{};
    glm::vec2 m_mouseDownPos{};
    glm::vec2 m_mouseMoveDelta{};
};
