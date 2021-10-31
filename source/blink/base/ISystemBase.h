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
#include <foundation/BaseTypesGlm.h>

NS_BEGIN

class ISystemBase
{
public:
    virtual bool initialize(flecs::world& world) = 0;
    virtual bool update(flecs::world& world) = 0;

};

NS_END
