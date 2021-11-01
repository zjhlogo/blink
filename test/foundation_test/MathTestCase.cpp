/*!
 * \file MathTestCase.cpp
 *
 * \author zjhlogo
 * \date 2020/06/05
 *
 *
 */
#include <foundation/BaseTypesGlm.h>
#include <glm/gtx/euler_angles.hpp>
#include <gtest/gtest.h>

void assertMatrixEqual(const glm::mat4& mat1, const glm::mat4& mat2)
{
    ASSERT_FLOAT_EQ(mat1[0][0], mat2[0][0]);
    ASSERT_FLOAT_EQ(mat1[0][1], mat2[0][1]);
    ASSERT_FLOAT_EQ(mat1[0][2], mat2[0][2]);
    ASSERT_FLOAT_EQ(mat1[0][3], mat2[0][3]);

    ASSERT_FLOAT_EQ(mat1[1][0], mat2[1][0]);
    ASSERT_FLOAT_EQ(mat1[1][1], mat2[1][1]);
    ASSERT_FLOAT_EQ(mat1[1][2], mat2[1][2]);
    ASSERT_FLOAT_EQ(mat1[1][3], mat2[1][3]);

    ASSERT_FLOAT_EQ(mat1[2][0], mat2[2][0]);
    ASSERT_FLOAT_EQ(mat1[2][1], mat2[2][1]);
    ASSERT_FLOAT_EQ(mat1[2][2], mat2[2][2]);
    ASSERT_FLOAT_EQ(mat1[2][3], mat2[2][3]);

    ASSERT_FLOAT_EQ(mat1[3][0], mat2[3][0]);
    ASSERT_FLOAT_EQ(mat1[3][1], mat2[3][1]);
    ASSERT_FLOAT_EQ(mat1[3][2], mat2[3][2]);
    ASSERT_FLOAT_EQ(mat1[3][3], mat2[3][3]);
}

TEST(MathTestCase, matrixTest)
{
    glm::mat4 matIdentity = blink::MAT4_IDENTITY;
    glm::mat4 matScale = glm::scale(blink::MAT4_IDENTITY, glm::vec3(4.0f, 5.0f, 6.0f));
    glm::mat4 matRotation = glm::eulerAngleXYZ(1.0f, 2.0f, 3.0f);
    glm::mat4 matTranslation = glm::translate(blink::MAT4_IDENTITY, glm::vec3(7.0f, 8.0f, 9.0f));
    glm::mat4 matSR = matRotation * matScale;
    glm::mat4 matRT = matTranslation * matRotation;
    glm::mat4 matSRT = matTranslation * matRotation * matScale;

    ASSERT_FLOAT_EQ(glm::length(matSRT[0]), 4.0f);
    ASSERT_FLOAT_EQ(glm::length(matSRT[1]), 5.0f);
    ASSERT_FLOAT_EQ(glm::length(matSRT[2]), 6.0f);

    {
        SCOPED_TRACE("Matrix Scale Rotate Test");
        glm::mat4 matCustomSR = matRotation;
        matCustomSR[0] *= matScale[0][0];
        matCustomSR[1] *= matScale[1][1];
        matCustomSR[2] *= matScale[2][2];
        assertMatrixEqual(matSR, matCustomSR);
    }

    {
        SCOPED_TRACE("Matrix Rotate Translate Test");
        glm::mat4 matCustomRT = matRotation;
        matCustomRT[3] = matTranslation[3];
        assertMatrixEqual(matRT, matCustomRT);
    }

    {
        SCOPED_TRACE("Matrix Scale Rotate Translate Test");
        glm::mat4 matCustomSRT = matRotation;
        matCustomSRT[0] *= matScale[0][0];
        matCustomSRT[1] *= matScale[1][1];
        matCustomSRT[2] *= matScale[2][2];
        matCustomSRT[3] = matTranslation[3];
        assertMatrixEqual(matSRT, matCustomSRT);
    }
}
