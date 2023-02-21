/*********************************************************************
 * \file   HelloWorldApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "HelloWorldApp.h"

#include <blink/blink.h>

bool HelloWorldApp::initializeLogicalSystems()
{
    return true;
}

bool HelloWorldApp::initializeRenderSystems()
{
    return true;
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(argc, argv, app);
}
