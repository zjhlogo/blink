/*!
 * \file blink.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "blink.h"
#include <RenderModule.h>

namespace blink
{
    void App::step(float dt)
    {
        m_ex.systems.update_all(dt);
    }

    int run(RenderModule* renderModule)
    {
        if (!renderModule->createDevice({ 1280, 720 }))
        {
            renderModule->destroyDevice();
            return -1;
        }

        while (renderModule->gameLoop())
        {
            // TODO: sleep ?
        }

        renderModule->destroyDevice();
        return 0;
    }
}
