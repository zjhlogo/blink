cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(hello_world)

set(SOURCE_FILES
HelloWorldApp.cpp
HelloWorldApp.h
)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libblink>
)

target_link_libraries(${PROJECT_NAME} libblink)

group_sources("${SOURCE_FILES}")
configure_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
