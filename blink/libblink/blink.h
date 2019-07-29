/*!
 * \file blink.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <entityx/entityx.h>

namespace blink
{
    class RenderModule;

    class App
    {
    public:
        virtual bool initialize() = 0;
        virtual void terminate() = 0;

        virtual void step(float dt);

    public:
        entityx::EntityX m_ex;

    };

    int run(RenderModule* renderModule);
}
