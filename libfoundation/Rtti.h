#pragma once

#include "BaseType.h"
#include <typeindex>

namespace blink
{
    class Rtti
    {
    public:
        Rtti(const std::type_index& typeIndex, const char* pszClassName, const Rtti* pBaseRtti)
            :m_typeIndex(typeIndex)
            , m_className(pszClassName)
            , m_pBaseRtti(pBaseRtti)
        {

        };

        ~Rtti() {};

        const std::type_index& getTypeIndex() const { return m_typeIndex; };
        const tstring& getClassName() const { return m_className; };
        const Rtti* getBaseRtti() const { return m_pBaseRtti; };

        bool isSame(const std::type_index& typeIndex) const
        {
            return (m_typeIndex == typeIndex);
        }

        bool isDerivedFrom(const std::type_index& baseTypeIndex) const
        {
            const Rtti* pRtti = this;

            while (pRtti)
            {
                if (pRtti->getTypeIndex() == baseTypeIndex) return true;
                pRtti = pRtti->getBaseRtti();
            }

            return false;
        }

    private:
        std::type_index m_typeIndex;
        tstring m_className;
        const Rtti* m_pBaseRtti{};

    };

    class NoRtti
    {
    public:
        NoRtti() {};
        ~NoRtti() {};

        static const Rtti* __RttiData() { return nullptr; };

    };
}

#define RTTI_DEF(this_class, super_class)                                                                          \
static const blink::Rtti* __RttiData()                                                                             \
{                                                                                                                  \
	static const blink::Rtti s_Rtti(std::type_index(typeid(this_class)), #this_class, super_class::__RttiData());  \
	return &s_Rtti;                                                                                                \
};                                                                                                                 \
virtual const blink::Rtti* getRtti() const override                                                                \
{                                                                                                                  \
	return this_class::__RttiData();                                                                               \
}

#define RTTI_ROOT(this_class)                                                                                      \
static const blink::Rtti* __RttiData()                                                                             \
{                                                                                                                  \
	static const blink::Rtti s_Rtti(std::type_index(typeid(this_class)), #this_class, NoRtti::__RttiData());       \
	return &s_Rtti;                                                                                                \
};                                                                                                                 \
virtual const blink::Rtti* getRtti() const                                                                         \
{                                                                                                                  \
	return this_class::__RttiData();                                                                               \
}
