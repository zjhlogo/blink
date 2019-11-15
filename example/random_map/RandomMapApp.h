#pragma once
#include <blink.h>

class RandomMapApp : public NS::App
{
public:
    RandomMapApp() {};
    virtual ~RandomMapApp() {};

	bool initialize() override;
	void terminate() override;

};
