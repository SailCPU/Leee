# Task Planning Module CMake Configuration
# 任务规划功能模块 - 智能编排和优化机器人任务

cmake_minimum_required(VERSION 3.16)

# Task Planning library
if(NOT TARGET task-planning)
    add_library(task-planning INTERFACE)

    target_include_directories(task-planning
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    set_target_properties(task-planning PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# Task orchestration component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/task-orchestration")
    add_subdirectory(task-orchestration)
endif()

# Path optimization component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/path-optimization")
    add_subdirectory(path-optimization)
endif()

# Dynamic scheduling component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/dynamic-scheduling")
    add_subdirectory(dynamic-scheduling)
endif()

# Task planning tests
if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

# Task planning examples
if(BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()
