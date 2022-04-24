
/*********************************************************************
 * \file   LineArtApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "LineArtApp.h"
#include "systems/LineArtEntityCreationSystem.h"

#include <blink/blink.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <render_systems/SceneRenderSystem.h>

bool LineArtApp::initialize()
{
    stdcpp_set_os_api();

    m_ecsWorld.addSystem(new LineArtEntityCreationSystem());
    if (!m_ecsWorld.initialize()) return false;

    // add render systems
    addRenderSystem(new SceneRenderSystem(this));
    if (!initializeRenderSystems()) return false;

    return true;
}

void LineArtApp::terminate()
{
    terminateRenderSystems();
    m_ecsWorld.terminate();
}

int main(int argc, char** argv)
{
    LineArtApp app;
    return blink::run(app);
}
