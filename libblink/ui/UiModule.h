#pragma once
#include "../core/Component.h"

namespace blink
{
    class UiModule : public Component
    {
    public:
        RTTI_DEF(UiModule, Component);

    public:
        UiModule();
        virtual ~UiModule();

        virtual bool initialize(Entity* pEntity) override;
        virtual void terminate() override;

        void begin();

        void end();
    };

}
