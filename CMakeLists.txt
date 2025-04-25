cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

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

# Add source files
set(SOURCES
    src/BinaryMessage.cpp
    src/MessageConfig.cpp
    src/BinaryMessageFactory.cpp
    src/FieldConfig.cpp
)

# Add library
add_library(BinaryMessageLibrary
    ${SOURCES}
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