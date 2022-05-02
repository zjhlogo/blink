cmake_minimum_required(VERSION 3.8)
include(${PROJECT_ROOT}/cmake/utils.cmake)

project(libglad)

set(SOURCE_FILES
KHR/khrplatform.h
glad.c
glad.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
