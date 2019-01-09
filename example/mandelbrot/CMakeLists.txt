cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(mandelbrot)

set(SOURCE_FILES
MandelbrotApp.cpp
MandelbrotApp.h
MandelbrotMaterial.cpp
MandelbrotMaterial.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "example")
