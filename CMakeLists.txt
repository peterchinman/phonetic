cmake_minimum_required(VERSION 3.15)
project(phonetic)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Detect or configure for Emscripten
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(EMSCRIPTEN ON)
    message(STATUS "Emscripten detected. Configuring for WebAssembly build.")
else()
    set(EMSCRIPTEN OFF)
    message(STATUS "Standard compiler detected. Configuring for native build.")
endif()

if(EMSCRIPTEN)
    # Set optimization flags for Release builds
    set(CMAKE_CXX_FLAGS "-O3")

    add_executable(phonetic src/phonetic.cpp)

    # Include headers
    target_include_directories(phonetic PUBLIC ${CMAKE_SOURCE_DIR}/include)

    # Add Emscripten-specific flags to output WASM
    set_target_properties(phonetic PROPERTIES
        LINK_FLAGS "-fexceptions -lembind -s INVOKE_RUN=0 -s MODULARIZE=1 --preload-file ${CMAKE_SOURCE_DIR}/data/CMUdict/cmudict-0.7b@/data/cmudict-0.7b -s EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]'"
    )
else()
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
endif()

if (SKBUILD)
    if (CMAKE_VERSION VERSION_LESS 3.18)
        set(DEV_MODULE Development)
    else()
        set(DEV_MODULE Development.Module)
    endif()
    find_package(Python 3.12 REQUIRED COMPONENTS Interpreter ${DEV_MODULE} OPTIONAL_COMPONENTS Development.SABIModule)

    if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()

    set_target_properties(phonetic PROPERTIES POSITION_INDEPENDENT_CODE ON)

    execute_process(
        COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
    find_package(nanobind CONFIG REQUIRED)

    nanobind_add_module(phoneticpy STABLE_ABI src/phoneticpy.cpp)
    target_include_directories(phoneticpy PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(phoneticpy PRIVATE phonetic)

    install(TARGETS phoneticpy LIBRARY DESTINATION phoneticpy)
endif()
