/*********************************************************************
 * \file   PhysicsApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "PhysicsApp.h"

#include <blink/blink.h>
#include <physics/JoltPhysicsSystem.h>

bool PhysicsApp::initializeLogicalSystems()
{
    addLogicSystem(new blink::JoltPhysicsSystem());

    return true;
}

bool PhysicsApp::initializeRenderSystems()
{
    return true;
}

int main(int argc, char** argv)
{
    PhysicsApp app;
    return blink::run(argc, argv, app);
}
