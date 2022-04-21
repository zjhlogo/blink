/**

    @file      blink.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "blink.h"

#include <core/modules/IRenderModule.h>
#include <core/modules/IResModule.h>

namespace blink
{
    int run(IApp& app)
    {
        auto renderModule = getRenderModule();
        auto resModule = getResModule();

        if (renderModule->createDevice({1280, 720}))
        {
            if (resModule->initialize())
            {
                if (app.initialize())
                {
                    while (renderModule->processEvent())
                    {
                        app.stepEcsWorld();

                        auto result = renderModule->render(
                            [&](IRenderData& renderData)
                            {
                                //
                                app.render(renderData);
                            });
                    }

                    renderModule->waitIdle();
                }

                app.terminate();
            }

            resModule->terminate();
        }

        renderModule->destroyDevice();
        return 0;
    }

} // namespace blink
