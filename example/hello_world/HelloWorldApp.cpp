/*!
 * \file HelloWorldApp.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "HelloWorldApp.h"

HelloWorldApp::HelloWorldApp()
{
}

HelloWorldApp::~HelloWorldApp()
{
}

bool HelloWorldApp::initialize()
{
    return true;
}

void HelloWorldApp::terminate()
{
}

int main(int argc, char** argv)
{
    return NS::run("Vulkan");
}
