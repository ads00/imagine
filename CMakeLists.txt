cmake_minimum_required(VERSION 3.12)

project(imagine)

set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE IG_SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/third_party/*.cpp"  "${PROJECT_SOURCE_DIR}/third_party/*.c"
        "${PROJECT_SOURCE_DIR}/third_party/*.hpp")

add_library(lib_imagine SHARED ${IG_SOURCES})
target_compile_definitions(lib_imagine PRIVATE IG_BUILD)
target_include_directories(lib_imagine PUBLIC "src" "third_party")

add_executable(exe_main model/main.cpp)
target_link_libraries(exe_main lib_imagine)