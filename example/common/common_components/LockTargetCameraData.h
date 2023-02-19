/*********************************************************************
 * \file   LockTargetCameraData.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once
#include <core/components/Components.h>

struct LockTargetCameraData
{
    blink::Position targetPos;
    glm::vec3 eulerRotation;
    float cameraDistance;
    float rotateSensitive;
};

