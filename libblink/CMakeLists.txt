cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)

project(libblink)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(tinyxml2 REQUIRED)

set(SOURCE_FILES
core/Component.cpp
core/Component.h
core/Entity.cpp
core/Entity.h
device/Device.h
render/geometry_list/BufferGeometry.cpp
render/geometry_list/BufferGeometry.h
render/BufferAttributes.cpp
render/BufferAttributes.h
render/Geometry.cpp
render/Geometry.h
render/GlConfig.h
render/Material.cpp
render/Material.h
render/Mesh.cpp
render/Mesh.h
render/RenderModule.cpp
render/RenderModule.h
render/RenderState.cpp
render/RenderState.h
render/Shader.cpp
render/Shader.h
render/Texture.cpp
render/Texture.h
Framework.cpp
Framework.h
IApp.cpp
IApp.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(SOURCE_FILES ${SOURCE_FILES} device/platform/windows/DeviceWindows.cpp)
endif ()

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:tinyxml2::tinyxml2,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libfoundation>
)

group_sources("${SOURCE_FILES}")
configure_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
