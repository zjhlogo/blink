cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(librender_vulkan)

set(SOURCE_FILES
utils/stb_image.h
utils/tiny_obj_loader.h
utils/VulkanUtils.cpp
utils/VulkanUtils.h
Types.cpp
Types.h
VulkanBuffer.cpp
VulkanBuffer.h
VulkanCommandBuffer.cpp
VulkanCommandBuffer.h
VulkanCommandPool.cpp
VulkanCommandPool.h
VulkanContext.cpp
VulkanContext.h
VulkanDescriptorPool.cpp
VulkanDescriptorPool.h
VulkanDescriptorSets.cpp
VulkanDescriptorSets.h
VulkanFence.cpp
VulkanFence.h
VulkanImage.cpp
VulkanImage.h
VulkanLogicalDevice.cpp
VulkanLogicalDevice.h
VulkanMemory.cpp
VulkanMemory.h
VulkanPipeline.cpp
VulkanPipeline.h
VulkanRenderModule.cpp
VulkanRenderModule.h
VulkanSemaphore.cpp
VulkanSemaphore.h
VulkanShader.cpp
VulkanShader.h
VulkanSwapchain.cpp
VulkanSwapchain.h
VulkanTexture.cpp
VulkanTexture.h
VulkanWindow.cpp
VulkanWindow.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
