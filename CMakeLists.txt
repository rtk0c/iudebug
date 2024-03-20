cmake_minimum_required(VERSION 3.13)
project(IuDebug LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
add_subdirectory(3rdparty/glad)

file(GLOB_RECURSE ImGui_SOURCES src/ImGui/*.c src/ImGui/*.cpp)
add_library(ImGui ${ImGui_SOURCES})
# ImGui stuff only ever use relative includes
#target_include_directories(ImGui PRIVATE src)
target_link_libraries(ImGui
PUBLIC
    Freetype::Freetype
    OpenGL::GL
    glfw
    glad
)

file(GLOB_RECURSE IuDebug_SOURCES src/IuDebug/*.c src/IuDebug/*.cpp)
add_executable(IuDebug ${IuDebug_SOURCES})
target_include_directories(IuDebug PRIVATE src/)
target_link_libraries(IuDebug
PRIVATE
    Threads::Threads
    ImGui
)

# Properties for everything!
set_target_properties(
    ImGui IuDebug
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
