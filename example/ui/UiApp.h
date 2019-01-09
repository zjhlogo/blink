#pragma once
#include <IApp.h>

class UiApp : public blink::IApp
{
public:
    RTTI_DEF(UiApp, blink::IApp);

    UiApp();
    virtual ~UiApp();

	virtual bool initialize() override;
	virtual void terminate() override;

    virtual void update(float dt) override;
    virtual void render() override;

private:
    float m_value{};
    char m_strValue[100]{};

};
