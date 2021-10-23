/*!
 * \file CameraData.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#pragma once

#include <flecs/flecs.h>

class ISystemBase
{
public:
    virtual bool initialize(flecs::world& world) = 0;

};
