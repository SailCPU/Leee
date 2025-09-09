# Leee Logger Module

Leee 项目的日志模块是一个基于 [plog](https://github.com/SergiusTheBest/plog) 库的 header-only 日志实现，提供 GLOG 风格的格式化和流式日志接口。

## 特性

- **Header-only**: 完全头文件实现，无需编译
- **GLOG 风格格式化**: 兼容 Google GLOG 的日志格式
- **流式接口**: 支持 `LOG_INFO << "message" << value` 语法
- **多输出**: 同时输出到控制台和文件
- **日志轮转**: 支持文件大小限制和历史文件数量限制
- **线程安全**: 支持多线程环境

## 快速开始

### 1. 包含头文件

```cpp
#include <logger.h>
```

### 2. 初始化日志

```cpp
// 使用默认配置
leee::init_logger();

// 或使用自定义日志文件
leee::init_logger("logs/my_app.log", 2000000, 10); // 2MB, 10个历史文件
```

### 3. 使用日志

```cpp
// 基础日志记录
LOG_INFO << "Application started";
LOG_WARNING << "This is a warning";
LOG_ERROR << "This is an error";

// 流式接口
int value = 42;
std::string name = "example";
LOG_INFO << "Processing " << name << " with value " << value;

// 不同日志级别
LOG_DEBUG << "Debug information";
LOG_VERBOSE << "Verbose information";
LOG_FATAL << "Fatal error occurred";
```

## 日志级别

- `LOG_VERBOSE`: 最详细的日志
- `LOG_DEBUG`: 调试信息
- `LOG_INFO`: 一般信息
- `LOG_WARNING`: 警告信息
- `LOG_ERROR`: 错误信息
- `LOG_FATAL`: 致命错误

## 日志格式

日志输出格式遵循 GLOG 风格：

```
I0823 14:30:45.123456 12345 main.cpp:25] Application started
```

格式说明：
- `I`: 日志级别 (I=Info, W=Warning, E=Error, F=Fatal)
- `0823`: 日期 (MMDD)
- `14:30:45.123456`: 时间 (HH:MM:SS.microseconds)
- `12345`: 线程ID
- `main.cpp:25`: 文件名和行号
- `Application started`: 日志消息

## 高级配置

### 设置日志级别

```cpp
    // 设置为只显示 INFO 及以上级别的日志
    leee::set_log_level(plog::info);

    // 获取当前日志级别
    auto current_level = leee::get_log_level();
```

### 检查初始化状态

```cpp
if (leee::is_initialized()) {
    LOG_INFO << "Logger is ready";
}
```

## 编译选项

确保包含路径包含：
- Leee 项目头文件路径
- plog 库路径 (位于 `header-only/plog/`)

### CMake 示例

```cmake
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/header-only
)
```

### 编译命令示例

```bash
g++ -std=c++17 -I./include -I./header-only -o example logger_example.cpp
```

## 示例代码

查看 `examples/logger_example.cpp` 获取完整的使用示例。

## 依赖

- C++17 或更高版本
- plog 库 (已包含在 `header-only/plog/` 中)

## 注意事项

1. 日志模块是线程安全的，可以在多线程环境中使用
2. 首次使用日志宏时会自动初始化日志系统
3. 日志文件默认保存在 `logs/` 目录中
4. 确保程序有权限创建日志目录和文件
