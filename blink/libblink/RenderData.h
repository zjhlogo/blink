#pragma once
#include "geometry/BufferGeometry.h"
#include "material/Material.h"
#include "light/Light.h"

namespace blink
{
    struct MeshData
    {
        MeshData(std::shared_ptr<BufferGeometry> geo, std::shared_ptr<Material> mat)
            :geometry(geo)
            , material(mat)
        {
        }

        ~MeshData()
        {
        }

        std::shared_ptr<BufferGeometry> geometry;
        std::shared_ptr<Material> material;
    };

    struct LightData
    {
        LightData(std::shared_ptr<Light> lt)
            : light(lt)
        {
        }

        ~LightData()
        {
        }

        std::shared_ptr<Light> light;
    };
}
