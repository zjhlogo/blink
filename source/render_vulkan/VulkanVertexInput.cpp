/*!
 * \file VulkanVertexInput.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanVertexInput.h"

NS_BEGIN

VulkanVertexInput::VulkanVertexInput(const BindingDescriptionList& bindingDescriptions, const AttributeDescriptionList& attributeDescriptions)
{
    m_bindingDescriptions = bindingDescriptions;
    m_attributeDescriptions = attributeDescriptions;
}

NS_END
