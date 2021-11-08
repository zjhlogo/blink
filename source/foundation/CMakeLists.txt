cmake_minimum_required(VERSION 3.8)

include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libfoundation)

set(SOURCE_FILES
BaseTypes.cpp
BaseTypes.h
BaseTypesGlm.cpp
BaseTypesGlm.h
BuiltinFormatter.h
Directory.cpp
Directory.h
File.cpp
File.h
Log.cpp
Log.h
PathParser.cpp
PathParser.h
PreDefine.h
StringBuilder.cpp
StringBuilder.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(SOURCE_FILES ${SOURCE_FILES}
		platform/windows/DirectoryWindows.cpp
		platform/windows/dirent_windows.h
		platform/windows/FileWindows.cpp
		platform/windows/LogWindows.cpp
	)
endif ()

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} libfmt libglm)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "blink")
