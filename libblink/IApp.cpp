#include "IApp.h"

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
}
