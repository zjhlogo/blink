cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(libflecs)

set(SOURCE_FILES
flecs.c
flecs.h
flecs_os_api_stdcpp_bake_config.h
flecs-os_api-stdcpp.cpp
flecs_os_api_stdcpp.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
