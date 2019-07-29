/*!
 * \file RenderData.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <BufferGeometry.h>
#include "material/Material.h"
#include "light/Light.h"

namespace blink
{
    struct MeshData
    {
        MeshData(BufferGeometry* geo, Material* mat)
            :geometry(geo)
            , material(mat)
        {
        }

        ~MeshData()
        {
        }

        BufferGeometry* geometry;
        Material* material;
    };

    struct LightData
    {
        LightData(Light* lt)
            : light(lt)
        {
        }

        ~LightData()
        {
        }

        Light* light;
    };
}
