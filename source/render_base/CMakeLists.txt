cmake_minimum_required(VERSION 3.8)

include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(librender_base)

set(SOURCE_FILES
util/ImageLoader.cpp
util/ImageLoader.h
util/stb_image.h
BufferAttributes.cpp
BufferAttributes.h
BufferGeometry.cpp
BufferGeometry.h
BufferObject.cpp
BufferObject.h
Geometry.cpp
Geometry.h
PredefinedVertexAttributes.h
RenderModule.cpp
RenderModule.h
Shader.cpp
Shader.h
Texture.cpp
Texture.h
VertexBuffer.cpp
VertexBuffer.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
