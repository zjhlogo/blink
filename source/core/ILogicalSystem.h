/**

    @file      ILogicalSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "EcsWorld.h"

#include <flecs/flecs.h>
#include <foundation/BaseTypesGlm.h>

namespace blink
{
    class ILogicalSystem
    {
    public:
        virtual bool initialize() = 0;
        virtual void terminate() = 0;

        virtual void framePreUpdate(){};
        virtual void framePostUpdate(){};

        void setEcsWorld(EcsWorld* ecsWorld) { m_ecsWorld = ecsWorld; };

    protected:
        EcsWorld* m_ecsWorld{};
    };

} // namespace blink
