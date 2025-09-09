# 模块结构设计

## 概述

Leee项目的模块化设计确保了代码的可维护性、可重用性和可扩展性。本文档定义了模块的组织结构、依赖关系和接口规范。

## 1. 模块组织原则

### 1.1 高内聚低耦合
- 模块内部功能高度相关
- 模块间依赖最小化
- 明确定义的模块边界
- 稳定的公共接口

### 1.2 职责分离
- 单一职责原则应用
- 关注点分离
- 接口与实现分离
- 配置与逻辑分离

## 2. 核心模块结构

### 2.1 命名空间层次
```
leee::
├── core::           # 核心功能
│   ├── memory       # 内存管理
│   ├── threading    # 线程管理
│   └── utilities    # 基础工具
├── containers::     # 容器类
│   ├── array        # 数组容器
│   ├── list         # 链表容器
│   └── map          # 映射容器
├── algorithms::     # 算法库
│   ├── sorting      # 排序算法
│   ├── searching    # 查找算法
│   └── numeric      # 数值算法
└── services::       # 服务组件
    ├── logging      # 日志服务
    ├── config       # 配置服务
    └── network      # 网络服务
```

### 2.2 目录结构
```
include/leee/
├── core/
│   ├── memory.h
│   ├── threading.h
│   └── utilities.h
├── containers/
│   ├── array.h
│   ├── list.h
│   └── map.h
├── algorithms/
│   ├── sorting.h
│   ├── searching.h
│   └── numeric.h
└── services/
    ├── logging.h
    ├── config.h
    └── network.h

src/
├── core/
├── containers/
├── algorithms/
└── services/

test/
├── core/
├── containers/
├── algorithms/
└── services/
```

## 3. 模块接口设计

### 3.1 头文件组织
每个模块的头文件应包含：
- 前置声明
- 类定义
- 内联函数
- 模板声明

```cpp
// memory.h
#pragma once

#include <memory>
#include <type_traits>

namespace leee {
namespace core {

// 前置声明
class MemoryPool;
template <typename T> class Allocator;

// 类定义
class MemoryPool {
public:
    static void* allocate(size_t size);
    static void deallocate(void* ptr);
};

// 模板类
template <typename T>
class Allocator {
public:
    using value_type = T;

    T* allocate(size_t n);
    void deallocate(T* p, size_t n);
};

} // namespace core
} // namespace leee
```

### 3.2 源文件组织
源文件实现头文件中声明的功能：
- 包含必要的头文件
- 实现类方法
- 模板实例化

```cpp
// memory.cpp
#include "memory.h"
#include <cstdlib>
#include <new>

namespace leee {
namespace core {

// 静态成员实现
void* MemoryPool::allocate(size_t size) {
    return std::malloc(size);
}

void MemoryPool::deallocate(void* ptr) {
    std::free(ptr);
}

// 模板实现
template <typename T>
T* Allocator<T>::allocate(size_t n) {
    return static_cast<T*>(
        MemoryPool::allocate(n * sizeof(T))
    );
}

template <typename T>
void Allocator<T>::deallocate(T* p, size_t n) {
    MemoryPool::deallocate(p);
}

// 显式模板实例化
template class Allocator<int>;
template class Allocator<std::string>;

} // namespace core
} // namespace leee
```

## 4. 模块依赖管理

### 4.1 依赖注入
使用依赖注入减少模块耦合：

```cpp
// 低耦合设计
class DatabaseConnection {
public:
    virtual ~DatabaseConnection() = default;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
};

class UserRepository {
public:
    UserRepository(std::unique_ptr<DatabaseConnection> connection)
        : connection_(std::move(connection)) {}

    User find_user(int id) {
        // 使用注入的连接
        return User{};
    }

private:
    std::unique_ptr<DatabaseConnection> connection_;
};
```

### 4.2 接口抽象
定义清晰的接口抽象：

```cpp
// 接口定义
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void info(const std::string& message) = 0;
    virtual void error(const std::string& message) = 0;
    virtual void set_level(LogLevel level) = 0;
};

// 具体实现
class ConsoleLogger : public ILogger {
public:
    void info(const std::string& message) override {
        std::cout << "[INFO] " << message << std::endl;
    }

    void error(const std::string& message) override {
        std::cerr << "[ERROR] " << message << std::endl;
    }

    void set_level(LogLevel level) override {
        // 实现设置日志级别
    }
};
```

## 5. 模块生命周期

### 5.1 初始化顺序
- 核心模块优先初始化
- 依赖模块后初始化
- 循环依赖检测和处理

### 5.2 清理顺序
- 逆初始化顺序清理
- 确保资源完全释放
- 处理清理过程中的异常

```cpp
class ModuleManager {
public:
    void initialize() {
        // 按依赖顺序初始化
        core_module_.initialize();
        container_module_.initialize();
        algorithm_module_.initialize();
        service_module_.initialize();
    }

    void shutdown() {
        // 逆序清理
        service_module_.shutdown();
        algorithm_module_.shutdown();
        container_module_.shutdown();
        core_module_.shutdown();
    }

private:
    CoreModule core_module_;
    ContainerModule container_module_;
    AlgorithmModule algorithm_module_;
    ServiceModule service_module_;
};
```

## 6. 模块测试架构

### 6.1 单元测试组织
每个模块有对应的测试文件：

```
test/
├── core/
│   ├── test_memory.cpp
│   └── test_threading.cpp
├── containers/
│   ├── test_array.cpp
│   └── test_map.cpp
├── algorithms/
│   ├── test_sorting.cpp
│   └── test_searching.cpp
└── services/
    ├── test_logging.cpp
    └── test_config.cpp
```

### 6.2 模拟对象
使用模拟对象进行单元测试：

```cpp
// 模拟数据库连接
class MockDatabaseConnection : public IDatabaseConnection {
public:
    MOCK_METHOD(bool, connect, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(bool, execute_query, (const std::string&), (override));
};

// 测试用例
TEST_CASE("UserRepository - Find User") {
    auto mock_db = std::make_unique<MockDatabaseConnection>();
    EXPECT_CALL(*mock_db, connect()).WillOnce(Return(true));
    EXPECT_CALL(*mock_db, execute_query(_)).WillOnce(Return(true));

    UserRepository repo(std::move(mock_db));
    auto user = repo.find_user(123);

    CHECK(user.id == 123);
}
```

## 7. 模块版本管理

### 7.1 版本号规范
使用语义化版本控制：
- 主版本号：不兼容的API变更
- 次版本号：向后兼容的功能增加
- 修订号：向后兼容的问题修复

### 7.2 兼容性保证
- 相同主版本号保证ABI兼容
- 向后兼容性保证
- 废弃API的渐进式移除

```cpp
// 版本宏定义
#define LEE_VERSION_MAJOR 1
#define LEE_VERSION_MINOR 0
#define LEE_VERSION_PATCH 0

// 版本检查宏
#define LEE_VERSION_CHECK(major, minor, patch) \
    ((LEE_VERSION_MAJOR > major) || \
     (LEE_VERSION_MAJOR == major && LEE_VERSION_MINOR > minor) || \
     (LEE_VERSION_MAJOR == major && LEE_VERSION_MINOR == minor && LEE_VERSION_PATCH >= patch))
```

## 8. 构建和包管理

### 8.1 CMake模块配置
每个模块有独立的CMakeLists.txt：

```cmake
# containers/CMakeLists.txt
add_library(leee_containers
    array.cpp
    list.cpp
    map.cpp
)

target_include_directories(leee_containers
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(leee_containers
    PUBLIC
        leee_core
)
```

### 8.2 包配置
使用CMake包配置文件：

```cmake
# LeeeConfig.cmake
include(CMakeFindDependencyMacro)

find_dependency(Threads)
find_dependency(Boost)

include("${CMAKE_CURRENT_LIST_DIR}/LeeeTargets.cmake")

set(Leee_LIBRARIES Leee::containers Leee::algorithms Leee::services)
set(Leee_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/../../../include")
```

## 9. 文档生成

### 9.1 模块文档
每个模块生成独立的文档：

```
docs/
├── core/
│   ├── memory.html
│   └── threading.html
├── containers/
│   ├── array.html
│   └── map.html
└── api-reference.html
```

### 9.2 Doxygen配置
为每个模块配置Doxygen：

```doxygen
# Doxyfile
PROJECT_NAME           = "Leee Core Module"
INPUT                  = ../include/leee/core
OUTPUT_DIRECTORY       = docs/core
RECURSIVE              = YES
EXTRACT_ALL            = YES
```

---

*良好的模块结构是高质量软件的基础。*
