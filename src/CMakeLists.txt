# Create the library
add_library(phonetic
    phonetic.cpp
)

# Define CMU_DICT_PATH based on the build type
if(EMSCRIPTEN)
    set(CMU_DICT_PATH "/data/cmudict-0.7b")
else()
    set(CMU_DICT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../data/CMUdict/cmudict-0.7b")
endif()
target_compile_definitions(phonetic PUBLIC CMU_DICT_PATH="${CMU_DICT_PATH}")

# Include header files
target_include_directories(phonetic PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

# Set properties for the library
set_target_properties(phonetic PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
