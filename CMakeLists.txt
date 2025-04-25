cmake_minimum_required(VERSION 3.10)
project(BinaryMessageLibrary VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set runtime library to match Google Test's default
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Download and configure nlohmann_json
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# Add library
add_library(BinaryMessageLibrary
    src/BinaryMessage.cpp
    src/MessageConfig.cpp
)

# Add public headers
target_include_directories(BinaryMessageLibrary
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(BinaryMessageLibrary
    PUBLIC
        nlohmann_json::nlohmann_json
)

# Add tests
enable_testing()
add_subdirectory(tests)

# Add examples
add_subdirectory(examples) 