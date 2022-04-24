cmake_minimum_required(VERSION 3.8)

project(blink)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prevent creates default "CMakePredefinedTargets" folder
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(source/foundation)
add_subdirectory(source/core)
add_subdirectory(source/physics)
add_subdirectory(source/render_vulkan)
add_subdirectory(source/blink)

add_subdirectory(example/common)
add_subdirectory(example/hello_world)
add_subdirectory(example/line_art)

add_subdirectory(external/flecs)
add_subdirectory(external/fmt)
add_subdirectory(external/glad)
add_subdirectory(external/glfw3)
add_subdirectory(external/glm)
add_subdirectory(external/gtest)
add_subdirectory(external/imgui)
add_subdirectory(external/noise)
add_subdirectory(external/spirv-cross)

add_subdirectory(test/foundation_test)
add_subdirectory(test/noise_test)
