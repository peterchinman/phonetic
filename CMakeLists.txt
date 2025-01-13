cmake_minimum_required(VERSION 3.14)
project(phonetic)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

option(FETCH_CATCH2 "Fetch Catch2 library for testing" ON)
# The parent project can disable this with:
# set(FETCH_CATCH2 OFF)

if (FETCH_CATCH2)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)
endif()

# Main application
add_subdirectory(src)

# Enable testing
enable_testing()

# Test directory
add_subdirectory(tests)
