# System Management Module CMake Configuration
# 系统管理功能模块 - 确保系统稳定可靠运行

cmake_minimum_required(VERSION 3.16)

# System Management library
if(NOT TARGET system-management)
    add_library(system-management INTERFACE)

    target_include_directories(system-management
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # Link to capability foundations for data processing and logging
    target_link_libraries(system-management INTERFACE nlohmann_json fmt plog)

    set_target_properties(system-management PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# Configuration management component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/configuration")
    add_subdirectory(configuration)
endif()

# Logging system component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/logging")
    add_subdirectory(logging)
endif()

# Health monitoring component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/health-monitoring")
    add_subdirectory(health-monitoring)
endif()

# System management tests
if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

# System management examples
if(BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()
