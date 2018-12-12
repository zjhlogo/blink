#include "Framework.h"
#include "IApp.h"
#include "device/Device.h"

namespace blink
{
    Framework& Framework::getInstance()
    {
        static Framework s_instance;
        return s_instance;
    }

    bool Framework::initialize(IApp* app)
    {
        m_app = app;

        if (!Device::initialize()) return false;

        return m_app->initialize();
    }

    int Framework::start()
    {
        int result = Device::start(std::bind(&Framework::step, this, std::placeholders::_1));
        if (result) return result;

        m_app->terminate();

        Device::terminate();

        return 0;
    }

    void Framework::step(float dt)
    {
        // TODO: 
    }

    void Framework::resize()
    {
    }

    void Framework::reload()
    {
    }
}
