#include "HelloWorldApp.h"
#include <Framework.h>

int blink::IApp::mainEntry()
{
	if (!blink::Framework::getInstance().initialize(new HelloWorldApp())) return -1;
	return blink::Framework::getInstance().start();
}

bool HelloWorldApp::initialize()
{
	return true;
}

void HelloWorldApp::terminate()
{

}
