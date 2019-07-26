cmake_minimum_required(VERSION 3.8)

project(blink)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(blink/libfoundation)
add_subdirectory(blink/libblink)
add_subdirectory(example/hello_world)
add_subdirectory(example/random_map)
add_subdirectory(external/glad)
add_subdirectory(external/imgui/source)
add_subdirectory(external/noise)
add_subdirectory(test/foundation_test)
add_subdirectory(test/noise_test)
