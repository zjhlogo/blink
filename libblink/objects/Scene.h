#pragma once
#include "Object.h"

namespace blink
{
    class Scene : public Object
    {
    public:
        RTTI_DEF(Scene, Object);

        Scene();
        virtual ~Scene();

    };
}
