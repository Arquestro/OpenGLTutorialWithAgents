cmake_minimum_required(VERSION 3.16)
project(OpenGLTutorialWithAgents LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 REQUIRED)

add_executable(glfw_starter
    src/main.cpp
)

target_link_libraries(glfw_starter PRIVATE glfw)
