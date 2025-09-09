# Perception Module CMake Configuration
# 智能感知功能模块 - 实时感知机器人状态和环境

cmake_minimum_required(VERSION 3.16)

# Perception library
if(NOT TARGET perception)
    add_library(perception INTERFACE)

    target_include_directories(perception
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # Link to capability foundations if needed
    # target_link_libraries(perception INTERFACE ...)

    set_target_properties(perception PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# State monitoring component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/state-monitoring")
    add_subdirectory(state-monitoring)
endif()

# Position sensing component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/position-sensing")
    add_subdirectory(position-sensing)
endif()

# Anomaly detection component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/anomaly-detection")
    add_subdirectory(anomaly-detection)
endif()

# Perception tests
if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

# Perception examples
if(BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()
