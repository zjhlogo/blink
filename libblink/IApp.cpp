#include "IApp.h"
#include "render/opengl3/OpenGL3RenderModule.h"
#include "input/MouseComponent.h"
#include "Framework.h"

namespace blink
{
    IApp::IApp(int windowWidth, int windowHeight, const tstring & title)
        :m_windowWidth(windowWidth)
        , m_windowHeight(windowHeight)
        , m_title(title)
    {

    }

    IApp::~IApp()
    {

    }

    bool IApp::initializeComponents()
    {
        if (!Framework::getInstance().insertComponent(createRenderer())) return false;
        if (!Framework::getInstance().insertComponent(new MouseComponent())) return false;

        return true;
    }

    void IApp::renderObject(Object* rootObj, Camera* camera)
    {
        RenderModule* pRenderModule = Framework::getInstance().findComponent<RenderModule>();
        if (!pRenderModule) return;

        pRenderModule->render(rootObj, camera);
    }

    RenderModule* IApp::createRenderer()
    {
        return new OpenGL3RenderModule();
    }
}
