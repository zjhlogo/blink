#pragma once

class IApp
{
public:
	virtual bool initialize() = 0;
	virtual void terminate() = 0;

	static int mainEntry();

};
