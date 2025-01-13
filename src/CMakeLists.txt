# Create the library
add_library(phonetic
    phonetic.cpp
)

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
