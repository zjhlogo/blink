cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libblink)

set(SOURCE_FILES
components/Components.h
geometries/builder/BoxBuilder.cpp
geometries/builder/BoxBuilder.h
geometries/builder/IGeometryBuilder.cpp
geometries/builder/IGeometryBuilder.h
geometries/builder/MeshBuilder.cpp
geometries/builder/MeshBuilder.h
geometries/builder/PlaneBuilder.cpp
geometries/builder/PlaneBuilder.h
geometries/builder/SphereUvBuilder.cpp
geometries/builder/SphereUvBuilder.h
geometries/builder/TetrahedronBuilder.cpp
geometries/builder/TetrahedronBuilder.h
geometries/Geometry.cpp
geometries/Geometry.h
materials/Material.cpp
materials/Material.h
resources/IResource.cpp
resources/IResource.h
resources/ResourceMgr.cpp
resources/ResourceMgr.h
textures/Texture2d.cpp
textures/Texture2d.h
type/RenderTypes.h
app.cpp
app.h
blink.cpp
blink.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libcore librender_vulkan libimgui)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
