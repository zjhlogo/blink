#pragma once
#include <Rtti.h>

namespace blink
{
    class IApp
    {
    public:
        RTTI_ROOT(IApp);

        IApp(int windowWidth, int windowHeight, const tstring& title);
        virtual ~IApp();

        virtual bool initialize() = 0;
        virtual void terminate() = 0;

        virtual void update(float dt) {};
        virtual void render() {};

        int getWindowWidth() const { return m_windowWidth; }
        int getWindowHeight() const { return m_windowHeight; }
        const tstring& getTitle() const { return m_title; }

    private:
        int m_windowWidth;
        int m_windowHeight;
        tstring m_title;

    };
}
