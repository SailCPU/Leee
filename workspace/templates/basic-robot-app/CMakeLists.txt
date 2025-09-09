# 基础机器人应用模板 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(basic_robot_app)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Leee平台（请根据实际安装路径修改）
find_package(Leee REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Leee_INCLUDE_DIRS}
)

# 源文件列表
set(SOURCES
    src/main.cpp
    src/robot_controller.cpp
    src/motion_planner.cpp
)

# 可执行文件
add_executable(basic_robot_app ${SOURCES})

# 链接Leee库
target_link_libraries(basic_robot_app
    Leee::motion-control
    Leee::perception
    Leee::system-management
)

# 编译选项
target_compile_options(basic_robot_app PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

# 包含测试
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# 安装目标
install(TARGETS basic_robot_app
    RUNTIME DESTINATION bin
)

# 安装配置文件
install(DIRECTORY config/
    DESTINATION share/basic_robot_app/config
    FILES_MATCHING PATTERN "*.yaml"
)

# 安装文档
install(DIRECTORY docs/
    DESTINATION share/basic_robot_app/docs
    FILES_MATCHING PATTERN "*.md"
)
