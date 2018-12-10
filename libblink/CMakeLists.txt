cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)

project(libblink)

find_package(glfw REQUIRED)

set(SOURCE_FILES
core/Component.cpp
core/Component.h
core/Entity.cpp
core/Entity.h
device/Device.h
Framework.cpp
Framework.h
IApp.cpp
IApp.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(SOURCE_FILES ${SOURCE_FILES} device/platform/windows/DeviceWindows.cpp)
endif ()

add_library(${PROJECT_NAME} ${SOURCE_FILES})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>>
	)
	target_link_libraries(${PROJECT_NAME} glfw opengl32)
endif ()

group_sources("${SOURCE_FILES}")
configure_project(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
