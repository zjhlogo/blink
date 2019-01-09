cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(normal_mapping)

set(SOURCE_FILES
NormalMappingApp.cpp
NormalMappingApp.h
NormalMappingMaterial.cpp
NormalMappingMaterial.h
)

group_sources("${SOURCE_FILES}")
configure_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin "example")
