#pragma once
#include <blink.h>

class HelloWorldApp : public blink::App
{
public:
    HelloWorldApp() {};
    virtual ~HelloWorldApp() {};

	bool initialize() override;
	void terminate() override;

};
