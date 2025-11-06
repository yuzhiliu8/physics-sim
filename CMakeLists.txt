cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(physics-engine)


add_executable(physics-engine
    src/main.cpp
    src/glad.c
)

target_include_directories(physics-engine PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL)
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARY})
