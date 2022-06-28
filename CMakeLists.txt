cmake_minimum_required(VERSION 3.13)
project(IuDebug LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
add_subdirectory(3rdparty/glad)

file(GLOB_RECURSE IuDebug_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/source/*.c
    ${CMAKE_CURRENT_LIST_DIR}/source/*.cpp
)
add_executable(IuDebug ${IuDebug_SOURCES})

target_include_directories(IuDebug PRIVATE sources/)
target_link_libraries(IuDebug
PRIVATE
    Threads::Threads
    OpenGL::GL
    glfw
    glad
)

target_compile_features(IuDebug PUBLIC cxx_std_20)
set_target_properties(IuDebug PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
