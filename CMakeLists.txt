cmake_minimum_required(VERSION 3.8)

project(blink)

add_subdirectory(external/glad)
add_subdirectory(libfoundation)
add_subdirectory(libblink)
add_subdirectory(example/hello_world)
add_subdirectory(example/mandelbrot)
add_subdirectory(test/foundation_test)
