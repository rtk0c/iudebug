cmake_minimum_required(VERSION 3.13)
project(iudebug LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
add_subdirectory(3rdparty/glad)

file(GLOB_RECURSE imgui_SOURCES src/imgui/*.c src/imgui/*.cpp)
add_library(imgui ${imgui_SOURCES})
# imgui stuff only ever use relative includes
#target_include_directories(imgui PRIVATE src)
target_link_libraries(imgui
PUBLIC
    Freetype::Freetype
    OpenGL::GL
    glfw
    glad
)

file(GLOB_RECURSE iudebug_SOURCES src/iudebug/*.c src/iudebug/*.cpp)
add_executable(iudebug ${iudebug_SOURCES})
target_include_directories(iudebug PRIVATE src)
target_link_libraries(iudebug
PRIVATE
    Threads::Threads
    imgui
)

# Properties for everything!
set_target_properties(
    imgui iudebug
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
