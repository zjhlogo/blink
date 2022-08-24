cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(libblink)

set(SOURCE_FILES
components/Components.h
geometries_builder/BoxBuilder.cpp
geometries_builder/BoxBuilder.h
geometries_builder/IGeometryBuilder.cpp
geometries_builder/IGeometryBuilder.h
geometries_builder/LineBuilder.h
geometries_builder/LineListBuilder.cpp
geometries_builder/LineListBuilder.h
geometries_builder/MeshBuilder.cpp
geometries_builder/MeshBuilder.h
geometries_builder/PlaneBuilder.cpp
geometries_builder/PlaneBuilder.h
geometries_builder/SphereUvBuilder.cpp
geometries_builder/SphereUvBuilder.h
geometries_builder/TetrahedronBuilder.cpp
geometries_builder/TetrahedronBuilder.h
systems/DebugLineSystem.cpp
systems/DebugLineSystem.h
types/RenderLayers.h
types/RenderOrders.h
utils/GltfUtil.cpp
utils/GltfUtil.h
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
