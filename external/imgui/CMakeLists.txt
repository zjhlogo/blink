cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libimgui)

set(SOURCE_FILES
backends/imgui_impl_glfw.cpp
backends/imgui_impl_glfw.h
backends/imgui_impl_vulkan.cpp
backends/imgui_impl_vulkan.h
imconfig.h
imgui.cpp
imgui.h
imgui_demo.cpp
imgui_draw.cpp
imgui_internal.h
imgui_tables.cpp
imgui_widgets.cpp
imstb_rectpack.h
imstb_textedit.h
imstb_truetype.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
