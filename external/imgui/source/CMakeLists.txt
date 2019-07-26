cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/utils.cmake)

project(libimgui)

find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

set(SOURCE_FILES
imgui/imconfig.h
imgui/imgui.cpp
imgui/imgui.h
imgui/imgui_demo.cpp
imgui/imgui_draw.cpp
imgui/imgui_impl_glfw.cpp
imgui/imgui_impl_glfw.h
imgui/imgui_impl_opengl3.cpp
imgui/imgui_impl_opengl3.h
imgui/imgui_internal.h
imgui/imgui_widgets.cpp
imgui/imstb_rectpack.h
imgui/imstb_textedit.h
imgui/imstb_truetype.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../glad/include>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
