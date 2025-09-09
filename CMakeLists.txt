# Leee Main CMake Configuration
# This is the main CMakeLists.txt for the Leee project - 功能驱动的机器人控制系统平台

cmake_minimum_required(VERSION 3.16)
project(leee
    VERSION 1.0.0
    DESCRIPTION "Leee - 智能机器人控制系统平台，专注于运动控制、感知计算、任务规划和人机协作"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Functional module build options - 功能模块构建选项
option(BUILD_MOTION_CONTROL "Build motion control module" ON)
option(BUILD_PERCEPTION "Build perception module" ON)
option(BUILD_TASK_PLANNING "Build task planning module" ON)
option(BUILD_HUMAN_INTERFACE "Build human interface module" ON)
option(BUILD_SYSTEM_MANAGEMENT "Build system management module" ON)
option(BUILD_INDUSTRIAL_INTEGRATION "Build industrial integration module" ON)
option(BUILD_DEVELOPMENT_TOOLS "Build development tools" ON)

# Development and testing options - 开发和测试选项
option(BUILD_TESTING "Build test executables" ON)
option(BUILD_EXAMPLES "Build example executables" ON)
option(BUILD_DOCUMENTATION "Build documentation" OFF)

# Include directories for the entire project - 功能导向的模块组织
include_directories(
    ${CMAKE_SOURCE_DIR}/capability-foundations
    ${CMAKE_SOURCE_DIR}/motion-control
    ${CMAKE_SOURCE_DIR}/perception
    ${CMAKE_SOURCE_DIR}/task-planning
    ${CMAKE_SOURCE_DIR}/human-interface
    ${CMAKE_SOURCE_DIR}/system-management
    ${CMAKE_SOURCE_DIR}/industrial-integration
)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

# Add functional module subdirectories conditionally - 条件性添加功能模块
if(BUILD_MOTION_CONTROL)
    add_subdirectory(motion-control)
endif()

if(BUILD_PERCEPTION)
    add_subdirectory(perception)
endif()

if(BUILD_TASK_PLANNING)
    add_subdirectory(task-planning)
endif()

if(BUILD_HUMAN_INTERFACE)
    add_subdirectory(human-interface)
endif()

if(BUILD_SYSTEM_MANAGEMENT)
    add_subdirectory(system-management)
endif()

if(BUILD_INDUSTRIAL_INTEGRATION)
    add_subdirectory(industrial-integration)
endif()

# Add development tools - 开发工具模块
if(BUILD_DEVELOPMENT_TOOLS)
    add_subdirectory(development-tools)
endif()

# Installation configuration
include(GNUInstallDirs)

# Install capability foundations - 能力基础库
install(DIRECTORY capability-foundations/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install functional module header files conditionally - 条件性安装功能模块头文件
if(BUILD_MOTION_CONTROL)
    install(DIRECTORY motion-control/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")
endif()

if(BUILD_PERCEPTION)
    install(DIRECTORY perception/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")
endif()

if(BUILD_TASK_PLANNING)
    install(DIRECTORY task-planning/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")
endif()

if(BUILD_HUMAN_INTERFACE)
    install(DIRECTORY human-interface/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")
endif()

if(BUILD_SYSTEM_MANAGEMENT)
    install(DIRECTORY system-management/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")
endif()

if(BUILD_INDUSTRIAL_INTEGRATION)
    install(DIRECTORY industrial-integration/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")
endif()

# Create package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LeeeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LeeeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LeeeConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Leee
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/LeeeConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/LeeeConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Leee
)

# Create uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()

# Functional module summary - 功能模块构建摘要
message(STATUS "Leee Functional Module Configuration:")
message(STATUS "  Motion Control: ${BUILD_MOTION_CONTROL}")
message(STATUS "  Perception: ${BUILD_PERCEPTION}")
message(STATUS "  Task Planning: ${BUILD_TASK_PLANNING}")
message(STATUS "  Human Interface: ${BUILD_HUMAN_INTERFACE}")
message(STATUS "  System Management: ${BUILD_SYSTEM_MANAGEMENT}")
message(STATUS "  Industrial Integration: ${BUILD_INDUSTRIAL_INTEGRATION}")
message(STATUS "  Development Tools: ${BUILD_DEVELOPMENT_TOOLS}")

# Development workflow targets - 开发工作流目标
add_custom_target(functional-overview
    COMMENT "Display functional module overview"
    COMMAND ${CMAKE_COMMAND} -E echo "Leee Functional Modules:"
    COMMAND ${CMAKE_COMMAND} -E echo "1. Motion Control - 运动控制能力"
    COMMAND ${CMAKE_COMMAND} -E echo "2. Perception - 智能感知能力"
    COMMAND ${CMAKE_COMMAND} -E echo "3. Task Planning - 任务规划能力"
    COMMAND ${CMAKE_COMMAND} -E echo "4. Human Interface - 人机协作界面"
    COMMAND ${CMAKE_COMMAND} -E echo "5. System Management - 系统管理功能"
    COMMAND ${CMAKE_COMMAND} -E echo "6. Industrial Integration - 工业集成功能"
    COMMAND ${CMAKE_COMMAND} -E echo "7. Development Tools - 开发工具集"
)

# Capability verification target - 能力验证目标
add_custom_target(verify-capabilities
    COMMENT "Verify that all enabled capabilities can be built"
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
)
