/*!
 * \file Reflect.h
 *
 * \author zjhlogo
 * \date 2020/01/23
 *
 *
 */
#pragma once
#include "BaseTypes.h"

#include <vector>

NS_BEGIN

//--------------------------------------------------------
// Base class of all type descriptors
//--------------------------------------------------------

struct TypeDescriptor
{
    const char* name;
    int size;

    TypeDescriptor(const char* name, int size);
    virtual ~TypeDescriptor();

    virtual tstring getFullName() const;
    virtual void dump(tstring& strOut, const void* obj, int indentLevel) const = 0;
};

//--------------------------------------------------------
// Finding type descriptors
//--------------------------------------------------------

// Declare the function template that handles primitive types such as int, tstring, etc.:
template <typename T> TypeDescriptor* getPrimitiveDescriptor();

// A helper class to find TypeDescriptors in different ways:
struct DefaultResolver
{
    template <typename T> static char func(decltype(&T::Reflection));
    template <typename T> static int func(...);
    template <typename T> struct IsReflected
    {
        enum
        {
            value = (sizeof(func<T>(nullptr)) == sizeof(char))
        };
    };

    // This version is called if T has a static member named "Reflection":
    template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0> static TypeDescriptor* get() { return &T::Reflection; }

    // This version is called otherwise:
    template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0> static TypeDescriptor* get() { return getPrimitiveDescriptor<T>(); }
};

// This is the primary class template for finding all TypeDescriptors:
template <typename T> struct TypeResolver
{
    static TypeDescriptor* get() { return DefaultResolver::get<T>(); }
};

//--------------------------------------------------------
// Type descriptors for user-defined structs/classes
//--------------------------------------------------------

struct TypeDescriptor_Struct : TypeDescriptor
{
    struct Member
    {
        const char* name;
        int offset;
        TypeDescriptor* type;
    };

    std::vector<Member> members;

    TypeDescriptor_Struct(void (*init)(TypeDescriptor_Struct*));
    TypeDescriptor_Struct(const char* name, int size, const std::initializer_list<Member>& init);

    virtual void dump(tstring& strOut, const void* obj, int indentLevel) const override;
};

#define REFLECT()                                                                                                                                              \
    friend struct NS::DefaultResolver;                                                                                                                         \
    static NS::TypeDescriptor_Struct Reflection;                                                                                                               \
    static void initReflection(NS::TypeDescriptor_Struct*)

#define REFLECT_STRUCT_BEGIN(type)                                                                                                                             \
    NS::TypeDescriptor_Struct type::Reflection{ type::initReflection };                                                                                        \
    void type::initReflection(NS::TypeDescriptor_Struct* typeDesc)                                                                                             \
    {                                                                                                                                                          \
        using T = type;                                                                                                                                        \
        typeDesc->name = #type;                                                                                                                                \
        typeDesc->size = sizeof(T);                                                                                                                            \
        typeDesc->members = {
#define REFLECT_STRUCT_MEMBER(name) { #name, offsetof(T, name), NS::TypeResolver<decltype(T::name)>::get() },

#define REFLECT_STRUCT_END()                                                                                                                                   \
    }                                                                                                                                                          \
    ;                                                                                                                                                          \
    }

//--------------------------------------------------------
// Type descriptors for std::vector
//--------------------------------------------------------

struct TypeDescriptor_StdVector : TypeDescriptor
{
    TypeDescriptor* itemType;
    int (*getSize)(const void*);
    const void* (*getItem)(const void*, int);

    template <typename ItemType>
    TypeDescriptor_StdVector(ItemType*)
        : TypeDescriptor{ "std::vector<>", sizeof(std::vector<ItemType>) }
        , itemType{ TypeResolver<ItemType>::get() }
    {
        getSize = [](const void* vecPtr) -> int {
            const auto& vec = *(const std::vector<ItemType>*)vecPtr;
            return vec.size();
        };
        getItem = [](const void* vecPtr, int index) -> const void* {
            const auto& vec = *(const std::vector<ItemType>*)vecPtr;
            return &vec[index];
        };
    }
    virtual tstring getFullName() const override;
    virtual void dump(tstring& strOut, const void* obj, int indentLevel) const override;
};

// Partially specialize TypeResolver<> for std::vectors:
template <typename T> class TypeResolver<std::vector<T>>
{
public:
    static TypeDescriptor* get()
    {
        static TypeDescriptor_StdVector typeDesc{ (T*)nullptr };
        return &typeDesc;
    }
};

NS_END
