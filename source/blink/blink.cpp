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
#include <core/modules/IResourceModule.h>
#include <foundation/File.h>
#include <foundation/PathParser.h>

namespace blink
{
    int run(int argc, char** argv, IApp& app)
    {
        PathParser parser;
        parser.parse(argv[0]);

        File::initializeRoot(PathParser::combinePath(parser.getDirectory(), "assets/"));

        auto renderModule = getRenderModule();
        auto resourceModule = getResourceModule();

        if (renderModule->createDevice({1280, 720}))
        {
            if (resourceModule->initialize())
            {
                if (app.initialize())
                {
                    while (renderModule->processEvent())
                    {
                        app.stepEcsWorld();
                        renderModule->render();
                    }

                    renderModule->waitIdle();
                }

                app.terminate();
            }

            resourceModule->terminate();
        }

        renderModule->destroyDevice();
        return 0;
    }

} // namespace blink
