cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libblink)

set(SOURCE_FILES
component/Components.h
geometry/builder/BoxBuilder.cpp
geometry/builder/BoxBuilder.h
geometry/builder/IGeometryBuilder.cpp
geometry/builder/IGeometryBuilder.h
geometry/builder/MeshBuilder.cpp
geometry/builder/MeshBuilder.h
geometry/builder/PlaneBuilder.cpp
geometry/builder/PlaneBuilder.h
geometry/builder/SphereUvBuilder.cpp
geometry/builder/SphereUvBuilder.h
geometry/builder/TetrahedronBuilder.cpp
geometry/builder/TetrahedronBuilder.h
geometry/Geometry.cpp
geometry/Geometry.h
material/Material.cpp
material/Material.h
resource/IResource.cpp
resource/IResource.h
resource/ResourceMgr.cpp
resource/ResourceMgr.h
texture/Texture2d.cpp
texture/Texture2d.h
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
