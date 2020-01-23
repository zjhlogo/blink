/*!
 * \file Reflect.cpp
 *
 * \author zjhlogo
 * \date 2020/01/23
 *
 *
 */
#include "Reflect.h"
#include "Log.h"

NS_BEGIN

TypeDescriptor::TypeDescriptor(const char* name, int size)
    : name(name)
    , size(size)
{
}

TypeDescriptor::~TypeDescriptor()
{
}

tstring TypeDescriptor::getFullName() const
{
    return name;
}

TypeDescriptor_Struct::TypeDescriptor_Struct(void (*init)(TypeDescriptor_Struct*))
    : TypeDescriptor(nullptr, 0)
{
    init(this);
}

TypeDescriptor_Struct::TypeDescriptor_Struct(const char* name, int size, const std::initializer_list<Member>& init)
    : TypeDescriptor(nullptr, 0)
    , members(init)
{
}

void TypeDescriptor_Struct::dump(tstring& strOut, const void* obj, int indentLevel) const
{
    tstring strIndent(4 * indentLevel, ' ');
    strOut += fmt::format("{0}\n{1}{{\n", name, strIndent);
    for (const Member& member : members)
    {
        tstring strIndentPlus(4 * (indentLevel + 1), ' ');
        strOut += fmt::format("{0}{1} = ", strIndentPlus, member.name);
        member.type->dump(strOut, (char*)obj + member.offset, indentLevel + 1);
    }

    strOut += fmt::format("{0}}}\n", strIndent);
}

tstring TypeDescriptor_StdVector::getFullName() const
{
    return tstring("std::vector<") + itemType->getFullName() + ">";
}

void TypeDescriptor_StdVector::dump(tstring& strOut, const void* obj, int indentLevel) const
{
    size_t numItems = getSize(obj);
    if (numItems == 0)
    {
        strOut += fmt::format("{0}{{}}\n", getFullName());
    }
    else
    {
        tstring strIndent(4 * indentLevel, ' ');

        strOut += fmt::format("{0}\n{1}{{\n", getFullName(), strIndent);
        for (size_t index = 0; index < numItems; index++)
        {
            tstring strIndentPlus(4 * (indentLevel + 1), ' ');
            strOut += fmt::format("{0}[{1}]", strIndentPlus, index);
            itemType->dump(strOut, getItem(obj, index), indentLevel + 1);
        }
        strOut += fmt::format("{0}}}\n", strIndent);
    }
}

//--------------------------------------------------------
// A type descriptor for int
//--------------------------------------------------------
struct TypeDescriptor_int : TypeDescriptor
{
    TypeDescriptor_int()
        : TypeDescriptor("int", sizeof(int))
    {
    }
    virtual void dump(tstring& strOut, const void* obj, int indentLevel) const override { strOut += fmt::format("int{{\"{0}\"}}\n", *(const int*)obj); }
};

template <> TypeDescriptor* getPrimitiveDescriptor<int>()
{
    static TypeDescriptor_int typeDesc;
    return &typeDesc;
}

//--------------------------------------------------------
// A type descriptor for tstring
//--------------------------------------------------------

struct TypeDescriptor_tstring : TypeDescriptor
{
    TypeDescriptor_tstring()
        : TypeDescriptor("tstring", sizeof(tstring))
    {
    }

    virtual void dump(tstring& strOut, const void* obj, int indentLevel) const override { strOut += fmt::format("tstring{{\"{0}\"}}\n", *(const tstring*)obj); }
};

template <> TypeDescriptor* getPrimitiveDescriptor<tstring>()
{
    static TypeDescriptor_tstring typeDesc;
    return &typeDesc;
}

NS_END
