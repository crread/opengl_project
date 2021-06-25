cmake_minimum_required(VERSION 3.17)
project(Dragon3DLight)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES Dragon3DLight.cpp)

include_directories(${PROJECT_SOURCE_DIR}/GLFW/include ${PROJECT_SOURCE_DIR}/glew/include)

link_directories(${PROJECT_SOURCE_DIR}/GLFW/lib ${PROJECT_SOURCE_DIR}/glew/lib)

add_executable(test_opengl ${SOURCE_FILES})

target_link_libraries(test_opengl opengl32 glew32 glfw3)