# Download and configure Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# Set runtime library for Google Test
if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(binary_message_tests
    test_main.cpp
    BinaryMessageTests.cpp
    MessageConfigTests.cpp
)

# Link test executable with Google Test and our library
target_link_libraries(binary_message_tests
    PRIVATE
        GTest::gtest_main
        BinaryMessageLibrary
)

# Add test executable to CTest
include(GoogleTest)
gtest_discover_tests(binary_message_tests) 