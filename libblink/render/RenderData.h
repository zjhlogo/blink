#pragma once
#include "geometry/BufferGeometry.h"
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
            SAFE_OBTAIN(geometry);
            SAFE_OBTAIN(material);
        }

        ~MeshData()
        {
            SAFE_RELEASE(geometry);
            SAFE_RELEASE(material);
        }

        BufferGeometry* geometry;
        Material* material;
    };

    struct LightData
    {
        LightData(Light* lt)
            : light(lt)
        {
            SAFE_OBTAIN(light);
        }

        ~LightData()
        {
            SAFE_RELEASE(light);
        }

        Light* light;
    };
}
