#include "IApp.h"
#include "render/opengl3/OpenGL3RenderModule.h"
#include "input/MouseComponent.h"
#include "ui/UiModule.h"
#include "Framework.h"
#include <glad/glad.h>

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
        if (!Framework::getInstance().insertComponent(new UiModule())) return false;

        return true;
    }

    void IApp::beginRender()
    {
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
