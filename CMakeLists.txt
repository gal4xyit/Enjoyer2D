cmake_minimum_required(VERSION 3.24)

project(Enjoyer2D
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENJOYER2D_BUILD_TESTBED "Build the SDL3/ImGui demo testbed" ON)
option(ENJOYER2D_BUILD_TESTS "Build Catch2 tests" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Dependencies)
include(ImGui)

add_subdirectory(engine)

if(ENJOYER2D_BUILD_TESTBED)
    add_subdirectory(testbed)
endif()

if(ENJOYER2D_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
