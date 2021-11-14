/**

    @file      SpirvCrossTestCase.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      14.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include <foundation/File.h>
#include <gtest/gtest.h>
#include <spirv-cross/spirv_glsl.hpp>

TEST(SpirvCrossTestCase, parseUniforms)
{
    std::vector<blink::uint8> buffer;
    blink::File::readFileIntoBuffer(buffer, "resource/shaders/simple_lit.vert.spv");

    spirv_cross::CompilerGLSL glsl((uint32_t*)buffer.data(), buffer.size() / sizeof(uint32_t));
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    // uniforms
    for (const auto& uniform : resources.uniform_buffers)
    {
        auto set = glsl.get_decoration(uniform.id, spv::DecorationDescriptorSet);
        auto binding = glsl.get_decoration(uniform.id, spv::DecorationBinding);
    }

    // samples
    for (const auto& sample : resources.sampled_images)
    {
    }
}
