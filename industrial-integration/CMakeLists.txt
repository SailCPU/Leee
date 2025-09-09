# Industrial Integration Module CMake Configuration
# 工业集成功能模块 - 无缝对接工业生产环境

cmake_minimum_required(VERSION 3.16)

# Industrial Integration library
if(NOT TARGET industrial-integration)
    add_library(industrial-integration INTERFACE)

    target_include_directories(industrial-integration
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # Link to capability foundations for communication
    target_link_libraries(industrial-integration INTERFACE Asio)

    set_target_properties(industrial-integration PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# Protocol adapters component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/protocol-adapters")
    add_subdirectory(protocol-adapters)
endif()

# Production lines component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/production-lines")
    add_subdirectory(production-lines)
endif()

# Quality control component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/quality-control")
    add_subdirectory(quality-control)
endif()

# Network communication component
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/network")
    add_subdirectory(network)
endif()

# Industrial integration tests
if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

# Industrial integration examples
if(BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()
