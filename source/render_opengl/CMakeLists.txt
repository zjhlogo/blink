cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(librender_opengl)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
shader_lib/ShaderLib.h
GlConfig.h
OpenGLBufferAttributes.cpp
OpenGLBufferAttributes.h
OpenGLBufferGeometry.cpp
OpenGLBufferGeometry.h
OpenGLBufferObject.cpp
OpenGLBufferObject.h
OpenGLRenderModule.cpp
OpenGLRenderModule.h
OpenGLShader.cpp
OpenGLShader.h
OpenGLVertexBuffer.cpp
OpenGLVertexBuffer.h
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
