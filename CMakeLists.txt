cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(physics-sim)


file(GLOB SRC_FILES "src/*.cpp" "src/*.c")

add_executable(physics-sim
    ${SRC_FILES}
)

target_include_directories(physics-sim PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL)
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARY})
