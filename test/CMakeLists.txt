# Leee Unit Tests CMake Configuration

cmake_minimum_required(VERSION 3.16)
project(leee_unit_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/../header-only
)

# ByteArray unit test executable
add_executable(test_bytearray test_bytearray.cpp)

# Compiler flags
if(MSVC)
    target_compile_options(test_bytearray PRIVATE /W4)
else()
    target_compile_options(test_bytearray PRIVATE -Wall -Wextra -pedantic)
endif()

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

# Enable testing
enable_testing()

# Add test
add_test(NAME ByteArrayUnitTest COMMAND test_bytearray)

# Set test properties
set_tests_properties(ByteArrayUnitTest PROPERTIES
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    ENVIRONMENT "PATH=${CMAKE_BINARY_DIR};$ENV{PATH}"
)
