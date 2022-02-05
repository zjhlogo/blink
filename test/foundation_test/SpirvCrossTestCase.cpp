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
    blink::File::readFileIntoBuffer(buffer, "resource/shaders/simple_lit.frag.spv");

    spirv_cross::CompilerGLSL glsl((uint32_t*)buffer.data(), buffer.size() / sizeof(uint32_t));
    auto resources = glsl.get_shader_resources();

    // uniforms
    for (const auto& uniform : resources.uniform_buffers)
    {
        auto name = uniform.name;

        auto type = glsl.get_type(uniform.type_id);
        auto baseType = glsl.get_type(uniform.base_type_id);

        auto set = glsl.get_decoration(uniform.id, spv::DecorationDescriptorSet);
        auto binding = glsl.get_decoration(uniform.id, spv::DecorationBinding);

        auto structSize = glsl.get_declared_struct_size(baseType);
        for (int i = 0; i < baseType.member_types.size(); ++i)
        {
            auto memberType = baseType.member_types[i];
            auto memberName = glsl.get_member_name(uniform.base_type_id, i);
        }
    }

    // samples
    for (const auto& sample : resources.sampled_images)
    {
        auto name = sample.name;
    }

    // input
    for (const auto& input : resources.stage_inputs)
    {
        auto name = input.name;

        auto type = glsl.get_type(input.type_id);
        auto baseType = glsl.get_type(input.base_type_id);

        auto location = glsl.get_decoration(input.id, spv::DecorationLocation);
        auto binding = glsl.get_decoration(input.id, spv::DecorationBinding);

    }

    // output
    for (const auto& output : resources.stage_outputs)
    {
        auto name = output.name;
        auto location = glsl.get_decoration(output.id, spv::DecorationLocation);
        auto type = glsl.get_type(output.type_id);
        auto baseType = glsl.get_type(output.base_type_id);
    }

    // builtin input
    for (const auto& input : resources.builtin_inputs)
    {
        auto name = input.resource.name;
        auto type = glsl.get_type(input.resource.type_id);
        auto baseType = glsl.get_type(input.resource.base_type_id);

        auto builtinType = input.builtin;
        auto valueType = glsl.get_type(input.value_type_id);
    }

    // builtin output
    for (const auto& output : resources.builtin_outputs)
    {
        auto name = output.resource.name;
        auto type = glsl.get_type(output.resource.type_id);
        auto baseType = glsl.get_type(output.resource.base_type_id);

        auto builtinType = output.builtin;
        auto valueType = glsl.get_type(output.value_type_id);
    }

    // variables
    auto variables = glsl.get_active_interface_variables();
    for (const auto var : variables)
    {
        auto name = glsl.get_name(var);
        const auto& type = glsl.get_type_from_variable(var);
        auto typeName = glsl.get_name(type.self);
    }
}
