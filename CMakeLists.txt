cmake_minimum_required(VERSION 3.8)

project(blink)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prevent creates default "CMakePredefinedTargets" folder
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")

add_subdirectory(blink/libfoundation)
add_subdirectory(blink/librender_base)
add_subdirectory(blink/librender_opengl)
add_subdirectory(blink/librender_vulkan)
add_subdirectory(blink/libblink)
add_subdirectory(example/hello_world)
add_subdirectory(example/random_map)
add_subdirectory(external/glad)
add_subdirectory(external/imgui/source)
add_subdirectory(external/noise)
add_subdirectory(test/foundation_test)
add_subdirectory(test/noise_test)
