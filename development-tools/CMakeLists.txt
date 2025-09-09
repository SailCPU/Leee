# Development Tools Module CMake Configuration
# 开发工具模块 - 加速开发和验证过程

cmake_minimum_required(VERSION 3.16)

# Development Tools main configuration
option(BUILD_TESTING "Build test executables" ON)
option(BUILD_EXAMPLES "Build example executables" ON)
option(BUILD_DOCUMENTATION "Build documentation" OFF)

# Link to testing framework
if(BUILD_TESTING)
    find_package(doctest QUIET)
    if(doctest_FOUND)
        # Use system doctest if available
    else()
        # Use bundled doctest
        if(NOT TARGET doctest)
            add_library(doctest INTERFACE)
            target_include_directories(doctest INTERFACE
                $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/capability-foundations/doctest>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            )
        endif()
    endif()
endif()

# Examples subdirectory
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()

# Testing subdirectory
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

# Simulation tools
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/simulation")
    add_subdirectory(simulation)
endif()

# Debugging tools
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/debugging")
    add_subdirectory(debugging)
endif()

# Scripts and build tools
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
    # Scripts are not built, just installed
    install(DIRECTORY scripts/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/leee/scripts
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                         GROUP_READ GROUP_EXECUTE
                         WORLD_READ WORLD_EXECUTE
    )
endif()

# Custom targets for development
if(BUILD_TESTING)
    add_custom_target(run-tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running all tests"
    )
endif()

if(BUILD_EXAMPLES)
    add_custom_target(run-examples
        COMMAND find ${CMAKE_BINARY_DIR} -name "*example*" -executable -exec {} \;
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running all examples"
    )
endif()

# Development workflow target
add_custom_target(dev-workflow
    COMMENT "Development workflow - build, test, and run examples"
)

if(TARGET run-tests)
    add_dependencies(dev-workflow run-tests)
endif()

if(TARGET run-examples)
    add_dependencies(dev-workflow run-examples)
endif()
