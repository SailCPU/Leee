# Leee Include Directory Structure

Leee 项目的头文件目录按照功能模块进行组织，便于管理和维护。

## 目录结构

```
include/
├── algorithm/     # 算法相关模块
├── network/       # 网络相关模块
├── utilities/     # 基础软件功能
│   ├── log/       # 日志模块
│   └── bytearray/ # 字符串工具模块
├── core/          # 核心基础功能
└── README.md      # 本说明文档
```

## 模块说明

### algorithm/
存放算法相关的模块和功能实现。

### network/
存放网络通信相关的模块和协议实现。

### utilities/
存放基础软件功能模块，如：
- **log/**: 日志模块
  - `logger.h`: 主要的日志头文件
  - `README.md`: 日志模块使用说明
- **bytearray/**: 字符串工具模块
  - `bytearray.h`: 字符串操作类头文件
  - `bytearray_impl.h`: 实现文件
  - `README.md`: 字符串工具使用说明

### core/
存放项目核心的基础功能模块。

## 使用说明

在使用这些模块时，请按照以下方式包含头文件：

```cpp
// 日志模块
#include <logger.h>

// 字符串工具模块
#include <bytearray.h>

// 算法模块（未来）
#include <algorithm/some_algorithm.h>

// 网络模块（未来）
#include <network/some_network.h>

// 核心模块（未来）
#include <core/some_core.h>
```

## 注意事项

1. 所有模块都是 header-only 设计，无需编译
2. 模块之间保持相对独立，便于单独使用
3. 遵循统一的命名空间规范：`leee::`
4. 文档会随着新模块的添加而更新

## 开发指南

- 新增算法相关功能 → `algorithm/` 目录
- 新增网络相关功能 → `network/` 目录
- 新增基础工具功能 → `utilities/` 目录
- 新增核心基础功能 → `core/` 目录

每个模块目录下应包含：
- 头文件 (`.h` 或 `.hpp`)
- 使用说明文档 (`README.md`)
- 示例代码 (可选，放在 `examples/` 目录)
