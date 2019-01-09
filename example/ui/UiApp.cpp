#include "UiApp.h"
#include <Framework.h>
#include <ui/UiModule.h>
#include <imgui.h>

UiApp::UiApp()
    :IApp(1280, 720, "Ui")
{

}

UiApp::~UiApp()
{

}

bool UiApp::initialize()
{
    return true;
}

void UiApp::terminate()
{

}

void UiApp::update(float dt)
{

}

void UiApp::render()
{
    auto pUiModule = blink::Framework::getInstance().findComponent<blink::UiModule>();
    if (!pUiModule) return;

    pUiModule->begin();

    ImGui::Text("Hello, world %d", 123);
    if (ImGui::Button("Save"))
    {
        // do stuff
    }
    ImGui::InputText("string", m_strValue, IM_ARRAYSIZE(m_strValue));
    ImGui::SliderFloat("float", &m_value, 0.0f, 1.0f);

    pUiModule->end();
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new UiApp());
}
