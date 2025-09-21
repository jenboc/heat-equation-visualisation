cmake_minimum_required(VERSION 3.14)
project(heateq)

set(CMAKE_CXX_20)
set(CMAKE_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Copy shaders and resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Include glfw with FetchContent
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLAD installed locally
add_subdirectory(lib/glad)

add_executable(heateq src/main.cpp src/shader.cpp)

find_package(OpenGL REQUIRED)
target_link_libraries(heateq PRIVATE glfw OpenGL::GL glad glm)
