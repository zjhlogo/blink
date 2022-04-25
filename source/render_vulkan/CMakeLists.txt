cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(librender_vulkan)

set(SOURCE_FILES
resources/VulkanGeometry.h
resources/VulkanGeometryLineList.cpp
resources/VulkanGeometryLineList.h
resources/VulkanGeometryTriangleList.cpp
resources/VulkanGeometryTriangleList.h
resources/VulkanMaterial.cpp
resources/VulkanMaterial.h
resources/VulkanTexture.cpp
resources/VulkanTexture.h
utils/VulkanUtils.cpp
utils/VulkanUtils.h
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
VulkanResModule.cpp
VulkanResModule.h
VulkanSemaphore.cpp
VulkanSemaphore.h
VulkanSwapchain.cpp
VulkanSwapchain.h
VulkanUniformBlock.cpp
VulkanUniformBlock.h
VulkanUniformBuffer.cpp
VulkanUniformBuffer.h
VulkanWindow.cpp
VulkanWindow.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libfoundation libglfw3 libspirv-cross)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
