cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libglad)

set(SOURCE_FILES
include/glad/glad.h
include/KHR/khrplatform.h
src/glad.c
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

group_sources("${SOURCE_FILES}")
configure_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
