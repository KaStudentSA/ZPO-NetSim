cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0135 NEW)
project(NetSim VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- ZASOBY (PLIKI) ---

# Zbieramy pliki źródłowe i nagłówkowe (.hxx z obrazka)
file(GLOB_RECURSE LIB_SOURCES
        "src/*.cpp"
        "include/*.hpp"
)

# Wykluczamy main.cpp z biblioteki Logic
list(FILTER LIB_SOURCES EXCLUDE REGEX "src/main.cpp$")

# Pliki testowe (odkomentuj testy jeśli potrzeba)
file(GLOB_RECURSE TEST_SOURCES
    tests/main_gtest.cpp
    #tests/test/test_Factory.cpp
    #tests/test/test_factory_io.cpp
    #tests/test/test_nodes.cpp
    tests/test_package.cpp
    #tests/test/test_reports.cpp
    #tests/test/test_simulate.cpp
    tests/test_storage_types.cpp
)

# --- CELE (TARGETS) ---

# Biblioteka Logic
add_library(Logic ${LIB_SOURCES})
target_include_directories(Logic PUBLIC include)

# Główny program
add_executable(Main src/main.cpp)
target_link_libraries(Main PRIVATE Logic)

# --- TESTY (GOOGLETEST) ---

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(Tests ${TEST_SOURCES})

target_link_libraries(
        Tests
        PRIVATE
        Logic
        gtest_main
        gmock
)

include(GoogleTest)
gtest_discover_tests(Tests)