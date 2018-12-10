#include "HelloWorldApp.h"
#include <Framework.h>

int IApp::mainEntry()
{
	if (!Framework::getInstance().initialize(new HelloWorldApp())) return -1;
	return Framework::getInstance().start();
}

bool HelloWorldApp::initialize()
{
	return true;
}

void HelloWorldApp::terminate()
{

}
