cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libblink)

set(SOURCE_FILES
base/ISystemBase.h
component/Components.h
geometry/builder/IGeometryBuilder.h
geometry/builder/PlaneBuilder.cpp
geometry/builder/PlaneBuilder.h
geometry/builder/SphereUvBuilder.cpp
geometry/builder/SphereUvBuilder.h
geometry/Box.cpp
geometry/Box.h
geometry/IGeometry.cpp
geometry/IGeometry.h
geometry/Mesh.cpp
geometry/Mesh.h
geometry/Plane.cpp
geometry/Plane.h
geometry/SphereUv.cpp
geometry/SphereUv.h
material/Material.cpp
material/material.h
app.cpp
app.h
blink.cpp
blink.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libfoundation libflecs librender_vulkan)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
