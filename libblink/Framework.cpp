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

    bool Framework::startup(IApp* app)
    {
        m_app = app;

        if (!Device::initialize(m_app->getWindowWidth(), m_app->getWindowHeight(), m_app->getTitle().c_str()))
        {
            shutdown();
            return false;
        }

        m_windowSize.x = static_cast<float>(m_app->getWindowWidth());
        m_windowSize.y = static_cast<float>(m_app->getWindowHeight());

        if (!m_app->initializeComponents())
        {
            shutdown();
            return false;
        }

        if (!m_app->initialize())
        {
            shutdown();
            return false;
        }

        Device::start();
        shutdown();
        return true;
    }

    void Framework::shutdown()
    {
        SAFE_DELETE_AND_TERMINATE(m_app);
        removeAllComponents();
        Device::terminate();
    }

    void Framework::step(float dt)
    {
        m_app->update(dt);

        m_app->beginRender();
        m_app->render();
        m_app->endRender();
    }
}
