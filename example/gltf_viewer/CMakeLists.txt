cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(gltf_viewer)

set(SOURCE_FILES
GltfViewerApp.cpp
GltfViewerApp.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${BIN_ROOT} "example")

add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND compile.bat
    WORKING_DIRECTORY ${RESOURCE_ROOT}/shaders
    USES_TERMINAL
    COMMENT "start compile shaders")
