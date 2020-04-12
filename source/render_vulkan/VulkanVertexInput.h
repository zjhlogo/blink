/*!
 * \file VulkanVertexInput.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanVertexInput
{
public:
    typedef std::vector<vk::VertexInputBindingDescription> BindingDescriptionList;
    typedef std::vector<vk::VertexInputAttributeDescription> AttributeDescriptionList;

public:
    VulkanVertexInput(const BindingDescriptionList& bindingDescriptions, const AttributeDescriptionList& attributeDescriptions);

    const BindingDescriptionList& getBindingDescriptions() const { return m_bindingDescriptions; }
    const AttributeDescriptionList& getAttributeDescriptions() const { return m_attributeDescriptions; }

private:
    BindingDescriptionList m_bindingDescriptions;
    AttributeDescriptionList m_attributeDescriptions;
};

NS_END
