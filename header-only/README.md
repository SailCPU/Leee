# Leee 第三方库 (Header-Only)

这个目录包含了Leee项目使用的所有第三方header-only库。

## 设计原则

- **Header-Only优先**: 优先选择header-only库以简化依赖管理和跨平台支持
- **版本管理**: 每个库都有明确的版本标识和许可证信息
- **最小化依赖**: 只包含实际使用的库，避免不必要的依赖
- **跨平台兼容**: 确保所有库都支持主流平台 (Windows, Linux, macOS)

## 包含的库

| 库名 | 版本 | 许可证 | 用途 |
|------|------|--------|------|
| [Asio](asio/) | 1.28.0 | BSL-1.0 | 异步I/O和网络编程 |
| [Eigen](eigen/) | 3.4.0 | MPL-2.0 | 线性代数计算 |
| [nlohmann/json](json/) | 3.11.2 | MIT | JSON解析和生成 |
| [fmt](fmt/) | 10.1.1 | MIT | 字符串格式化 |
| [plog](plog/) | 1.1.9 | MIT | 日志记录 |
| [doctest](doctest/) | 2.4.11 | MIT | 单元测试框架 |
| [httplib](httplib/) | 0.14.1 | MIT | HTTP客户端/服务器 |
| [WebSocket++](websocketpp/) | 0.8.2 | BSD | WebSocket协议 |

## 使用方式

### 在CMake中包含

```cmake
# 在CMakeLists.txt中添加包含路径
include_directories(${CMAKE_SOURCE_DIR}/header-only)

# 或者使用target_include_directories
target_include_directories(your_target PRIVATE ${CMAKE_SOURCE_DIR}/header-only)
```

### 在代码中包含

```cpp
// Asio网络编程
#include <asio.hpp>

// JSON处理
#include <nlohmann/json.hpp>

// 字符串格式化
#include <fmt/format.h>

// 日志记录
#include <plog/Log.h>
```

## 版本更新流程

1. 检查上游库的最新版本
2. 下载新版本的header文件
3. 更新对应库的`metadata.json`文件
4. 测试兼容性
5. 更新此README.md文件
6. 提交更改并标注版本信息

## 注意事项

- 所有库都是header-only，不需要编译
- 确保编译器支持C++17标准
- 某些库可能有额外的系统依赖（如Asio需要系统库）
- 定期检查安全更新和漏洞修复

## 许可证兼容性

所有包含的第三方库都使用兼容的开源许可证，确保Leee项目的整体许可证兼容性。
