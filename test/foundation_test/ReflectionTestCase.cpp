/*!
 * \file ReflectionTestCase.cpp
 *
 * \author zjhlogo
 * \date 2020/01/23
 *
 *
 */
#include <foundation/Reflect.h>
#include <gtest/gtest.h>

class ClassA
{
public:
    NS::tstring key;
    int value;
    std::vector<ClassA> children;

    REFLECT();
};

REFLECT_STRUCT_BEGIN(ClassA)
REFLECT_STRUCT_MEMBER(key)
REFLECT_STRUCT_MEMBER(value)
REFLECT_STRUCT_MEMBER(children)
REFLECT_STRUCT_END()

TEST(ReflectionTestCase, enableReflect)
{
    NS::TypeDescriptor* typeDesc = NS::TypeResolver<ClassA>::get();

    ClassA clsA{"main", 1, {{"inst1", 2, {}}, {"inst2", 3, {}}}};

    NS::tstring strDump;
    typeDesc->dump(strDump, &clsA, 0);

    ASSERT_EQ(typeDesc->getFullName(), "ClassA");
}
