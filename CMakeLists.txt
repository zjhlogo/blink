cmake_minimum_required(VERSION 3.8)

project(blink)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(external/glad)
add_subdirectory(external/imgui)
add_subdirectory(libfoundation)
add_subdirectory(libblink)
add_subdirectory(example/hello_world)
add_subdirectory(test/foundation_test)
