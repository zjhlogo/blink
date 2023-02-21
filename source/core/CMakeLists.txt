cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(libcore)

set(SOURCE_FILES
components/Components.h
modules/IRenderModule.cpp
modules/IRenderModule.h
modules/IResourceModule.cpp
modules/IResourceModule.h
resources/IGeometry.cpp
resources/IGeometry.h
resources/IMaterial.cpp
resources/IMaterial.h
resources/IResource.h
resources/ITexture.h
resources/ITexture2d.cpp
resources/ITexture2d.h
systems/ILogicalSystem.h
systems/IRenderSystem.h
types/Color.cpp
types/Color.h
types/UniformTypes.h
types/VertexAttrs.h
EcsWorld.cpp
EcsWorld.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libfoundation libflecs libphysics)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
