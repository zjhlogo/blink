#pragma once
#include <functional>

class Device
{
public:
	typedef std::function<void(float dt)> UpdateCb;

public:
	static bool initialize();
	static void terminate();
	static int start(const UpdateCb& cb);
};
