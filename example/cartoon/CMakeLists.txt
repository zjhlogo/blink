cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(cartoon)

set(SOURCE_FILES
CartoonApp.cpp
CartoonApp.h
CartoonMaterial.cpp
CartoonMaterial.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "example")
