#pragma once
#include <IApp.h>

class HelloWorldApp : public blink::IApp
{
public:
	virtual bool initialize() override;
	virtual void terminate() override;

};
