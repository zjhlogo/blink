#pragma once
#include <functional>

namespace blink
{
    class Device
    {
    public:
        typedef std::function<void(float dt)> UpdateCb;

    public:
        static bool initialize(int width, int height, const char* title);
        static void terminate();
        static int start();
    };
}
