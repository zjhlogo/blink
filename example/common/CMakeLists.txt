cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(libcommon)

set(SOURCE_FILES
guis/IGuiWindow.h
render_systems/ImguiRenderSystem.cpp
render_systems/ImguiRenderSystem.h
render_systems/SceneRenderSystem.cpp
render_systems/SceneRenderSystem.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libblink)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
	$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "example")
