/*!
 * \file StringBuilderTestCase.cpp
 *
 * \author zjhlogo
 * \date 2020/01/23
 *
 *
 */
#include <foundation/StringBuilder.h>
#include <gtest/gtest.h>

TEST(StringBuilderTestCase, replaceTest)
{
    blink::tstring str = "hello world!";
    ASSERT_EQ(blink::StringBuilder::replace(str, "l", "_"), 3);
    ASSERT_EQ(str, "he__o wor_d!");
}
