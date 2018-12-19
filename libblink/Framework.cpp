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

    void Framework::terminate()
    {
        SAFE_DELETE(m_app);
        removeAllComponents();
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
        m_app->update(dt);
        m_app->render();
    }

    void Framework::resize()
    {
    }

    void Framework::reload()
    {
    }
}
