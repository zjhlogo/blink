cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(librender_vulkan)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
utils/VulkanUtils.cpp
utils/VulkanUtils.h
Types.cpp
Types.h
VulkanBuffer.cpp
VulkanBuffer.h
VulkanCommandPool.cpp
VulkanCommandPool.h
VulkanContext.cpp
VulkanContext.h
VulkanDescriptorPool.cpp
VulkanDescriptorPool.h
VulkanDescriptorSets.cpp
VulkanDescriptorSets.h
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
VulkanShader.cpp
VulkanShader.h
VulkanSwapchain.cpp
VulkanSwapchain.h
VulkanTexture.cpp
VulkanTexture.h
VulkanVertexInput.cpp
VulkanVertexInput.h
VulkanWindow.cpp
VulkanWindow.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glm::glm,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
