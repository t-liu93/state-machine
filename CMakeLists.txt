cmake_minimum_required(VERSION 3.10)
project(state-machine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_library(state-machine INTERFACE
    include/statemachine.hpp
)

add_executable(state-machine-test
    tests/test_statemachine.cpp
)

target_include_directories(state-machine-test PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(state-machine-test PRIVATE
    state-machine
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(state-machine-test)