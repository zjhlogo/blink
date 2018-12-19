#pragma once
#include <functional>

class Device
{
public:
	typedef std::function<void(float dt)> UpdateCb;

public:
	static bool initialize(int width, int height, const char* title);
	static void terminate();
	static int start(const UpdateCb& cb);
};
