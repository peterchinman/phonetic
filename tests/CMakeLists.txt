# Add an executable for the test
add_executable(test_phonetic
    test_phonetic.cpp
)

# Link the library and Catch2
target_link_libraries(test_phonetic PRIVATE
    phonetic
    Catch2::Catch2WithMain
)

# Register the test with CTest
add_test(NAME test_phonetic COMMAND test_phonetic)
