/*!
 * \file RenderData.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include "light/Light.h"
#include "material/Material.h"

#include <render_base/BufferGeometry.h>

NS_BEGIN

struct MeshData
{
    MeshData(BufferGeometry* geo, Material* mat)
        : geometry(geo)
        , material(mat)
    {
    }

    ~MeshData() {}

    BufferGeometry* geometry;
    Material* material;
};

struct LightData
{
    LightData(Light* lt)
        : light(lt)
    {
    }

    ~LightData() {}

    Light* light;
};

NS_END
