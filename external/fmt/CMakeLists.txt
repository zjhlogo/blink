cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libfmt)

set(SOURCE_FILES
args.h
chrono.h
color.h
compile.h
core.h
format.cc
format.h
format-inl.h
locale.h
os.cc
os.h
ostream.h
printf.h
ranges.h
xchar.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
