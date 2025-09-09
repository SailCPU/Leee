# Leee Logger Example CMake Configuration

cmake_minimum_required(VERSION 3.16)
project(leee_logger_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/../header-only
)

# Logger example executable
add_executable(logger_example logger_example.cpp)

# ByteArray example executable
add_executable(bytearray_example bytearray_example.cpp)

# Compiler flags
if(MSVC)
    target_compile_options(logger_example PRIVATE /W4)
    target_compile_options(bytearray_example PRIVATE /W4)
else()
    target_compile_options(logger_example PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(bytearray_example PRIVATE -Wall -Wextra -pedantic)
endif()

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)
