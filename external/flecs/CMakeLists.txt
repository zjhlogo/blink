cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libflecs)

set(SOURCE_FILES
flecs.c
flecs.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
