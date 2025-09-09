# Human Interface Module CMake Configuration
# 人机协作界面模块 - 提供直观易用的操作界面

cmake_minimum_required(VERSION 3.16)

# Human Interface library
if(NOT TARGET human-interface)
    add_library(human-interface INTERFACE)

    target_include_directories(human-interface
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # Link to capability foundations for web and communication
    target_link_libraries(human-interface INTERFACE httplib WebSocket++)

    set_target_properties(human-interface PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# Remote monitoring component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/remote-monitoring")
    add_subdirectory(remote-monitoring)
endif()

# Intuitive control component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/intuitive-control")
    add_subdirectory(intuitive-control)
endif()

# Data visualization component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data-visualization")
    add_subdirectory(data-visualization)
endif()

# Web interface component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/web")
    add_subdirectory(web)
endif()

# Human interface tests
if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

# Human interface examples
if(BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()
