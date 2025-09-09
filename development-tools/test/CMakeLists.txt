# Leee Unit Tests CMake Configuration

cmake_minimum_required(VERSION 3.16)
project(leee_unit_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/capability-foundations
)

# Test executables
add_executable(test_bytearray test_bytearray.cpp)
target_link_libraries(test_bytearray PRIVATE doctest)
add_executable(test_logger test_logger.cpp)
target_link_libraries(test_logger PRIVATE doctest)
add_executable(test_performance test_performance.cpp)
target_link_libraries(test_performance PRIVATE doctest)
add_executable(test_integration test_integration.cpp)
target_link_libraries(test_integration PRIVATE doctest)
add_executable(test_edge_cases test_edge_cases.cpp)
target_link_libraries(test_edge_cases PRIVATE doctest)
add_executable(test_report_generator test_report_generator.cpp)
target_link_libraries(test_report_generator PRIVATE doctest)

# Compiler flags for all test executables
if(MSVC)
    set(TEST_COMPILE_FLAGS /W4)
else()
    set(TEST_COMPILE_FLAGS -Wall -Wextra -pedantic)
endif()

target_compile_options(test_bytearray PRIVATE ${TEST_COMPILE_FLAGS})
target_compile_options(test_logger PRIVATE ${TEST_COMPILE_FLAGS})
target_compile_options(test_performance PRIVATE ${TEST_COMPILE_FLAGS})
target_compile_options(test_integration PRIVATE ${TEST_COMPILE_FLAGS})
target_compile_options(test_edge_cases PRIVATE ${TEST_COMPILE_FLAGS})
target_compile_options(test_report_generator PRIVATE ${TEST_COMPILE_FLAGS})

# Link doctest for report generator (if needed)
# target_link_libraries(test_report_generator doctest)

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

# Enable testing
enable_testing()

# Add tests
add_test(NAME ByteArrayUnitTest COMMAND test_bytearray)
add_test(NAME LoggerUnitTest COMMAND test_logger)
add_test(NAME PerformanceTest COMMAND test_performance)
add_test(NAME IntegrationTest COMMAND test_integration)
add_test(NAME EdgeCasesTest COMMAND test_edge_cases)
add_test(NAME ReportGeneratorTest COMMAND test_report_generator)

# Set test properties for all tests
set_tests_properties(
    ByteArrayUnitTest
    LoggerUnitTest
    PerformanceTest
    IntegrationTest
    EdgeCasesTest
    ReportGeneratorTest
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    ENVIRONMENT "PATH=${CMAKE_BINARY_DIR};$ENV{PATH}"
)

# Test labels for organization
set_tests_properties(ByteArrayUnitTest PROPERTIES LABELS "unit")
set_tests_properties(LoggerUnitTest PROPERTIES LABELS "unit")
set_tests_properties(PerformanceTest PROPERTIES LABELS "performance")
set_tests_properties(IntegrationTest PROPERTIES LABELS "integration")
set_tests_properties(EdgeCasesTest PROPERTIES LABELS "edge_cases")
set_tests_properties(ReportGeneratorTest PROPERTIES LABELS "reporting")
