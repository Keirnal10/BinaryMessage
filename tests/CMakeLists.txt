# Download and configure Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# Set runtime library for Google Test
if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(BinaryMessageTests
    test_main.cpp
    BinaryMessageTests.cpp
    BinaryMessageFactoryTests.cpp
    MessageConfigTests.cpp
)

# Link test executable with Google Test and our library
target_link_libraries(BinaryMessageTests
    PRIVATE
        BinaryMessageLibrary
        GTest::gtest_main
)

# Add test executable to CTest
include(GoogleTest)
gtest_discover_tests(BinaryMessageTests) 