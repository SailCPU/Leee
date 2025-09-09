# Motion Control Module CMake Configuration
# 运动控制功能模块 - 实现机器人精准、高效的运动能力

cmake_minimum_required(VERSION 3.16)

# Motion Control library
if(NOT TARGET motion-control)
    add_library(motion-control INTERFACE)

    target_include_directories(motion-control
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # Link to capability foundations
    target_link_libraries(motion-control INTERFACE Eigen)

    # Set properties
    set_target_properties(motion-control PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# Servo control component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/servo-control")
    add_subdirectory(servo-control)
endif()

# Trajectory planning component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/trajectory-planning")
    add_subdirectory(trajectory-planning)
endif()

# Force control component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/force-control")
    add_subdirectory(force-control)
endif()

# Motion control tests
if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

# Motion control examples
if(BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()
