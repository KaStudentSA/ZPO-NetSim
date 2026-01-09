cmake_minimum_required(VERSION 3.14)
project(NetSim VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE ALL_SRC "src/*.cpp")
set(LIB_SOURCES ${ALL_SRC})
list(FILTER LIB_SOURCES EXCLUDE REGEX "src/main.cpp$")

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")


add_library(Logic ${LIB_SOURCES})
target_include_directories(Logic PUBLIC include)


add_executable(Main src/main.cpp)
target_link_libraries(Main PRIVATE Logic)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests
        PRIVATE
        Logic
        GTest::gtest
        GTest::gmock
)


include(GoogleTest)
gtest_discover_tests(Tests)