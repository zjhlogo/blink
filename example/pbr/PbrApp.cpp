/*********************************************************************
 * \file   PbrApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "PbrApp.h"
#include "blink/blink.h"
#include "common/render_systems/ImguiRenderSystem.h"
#include "common/utils/SceneEntityUtil.h"

bool PbrApp::initializeLogicalSystems()
{
    SceneEntityUtil::initializeCommonLogicalSystems(this);
    return true;
}

bool PbrApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(this);
    return true;
}

void PbrApp::onGui()
{
}

bool PbrApp::startup()
{
    auto& ecsWorld = getEcsWorld();
    if (!SceneEntityUtil::initializeCommonSceneEntities(ecsWorld))
    {
        return false;
    }

    return true;
}

void PbrApp::shutdown()
{
}

int main(int argc, char** argv)
{
    PbrApp app;
    return blink::run(argc, argv, app);
}
