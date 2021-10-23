cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libgtest)

set(SOURCE_FILES
internal/custom/gtest.h
internal/custom/gtest-port.h
internal/custom/gtest-printers.h
internal/gtest-death-test-internal.h
internal/gtest-filepath.h
internal/gtest-internal.h
internal/gtest-param-util.h
internal/gtest-port.h
internal/gtest-port-arch.h
internal/gtest-string.h
internal/gtest-type-util.h
dymmy.c
gtest.h
gtest_pred_impl.h
gtest_prod.h
gtest-death-test.h
gtest-matchers.h
gtest-message.h
gtest-param-test.h
gtest-printers.h
gtest-spi.h
gtest-test-part.h
gtest-typed-test.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} debug ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtestd.lib)
target_link_libraries(${PROJECT_NAME} optimized ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest.lib)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
