# 工业机器人焊接应用 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)
project(industrial-robot-welding)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Leee平台
find_package(Leee REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Leee_INCLUDE_DIRS}
)

# 源文件
set(SOURCES
    src/main.cpp
    src/welding_control.cpp
)

# 创建可执行文件
add_executable(welding_demo ${SOURCES})

# 链接Leee库
target_link_libraries(welding_demo
    Leee::motion-control
    Leee::perception
    Leee::task-planning
    Leee::system-management
    Leee::industrial-integration
)

# 设置编译选项
target_compile_options(welding_demo PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

# 安装目标
install(TARGETS welding_demo
    RUNTIME DESTINATION bin
)

# 复制配置文件
install(DIRECTORY config/
    DESTINATION share/industrial-robot/config
    FILES_MATCHING PATTERN "*.yaml"
)

# 复制文档
install(DIRECTORY docs/
    DESTINATION share/industrial-robot/docs
    FILES_MATCHING PATTERN "*.md"
)
