# API文档规范

## 概述

完善的API文档是开发者使用库的关键。本文档定义了Leee项目的API文档编写规范、使用Doxygen生成文档的标准和最佳实践。

## 1. 文档生成工具

### 1.1 Doxygen配置
使用Doxygen生成API文档：

```doxygen
# Doxyfile配置
PROJECT_NAME           = "Leee C++ Library"
PROJECT_NUMBER         = 1.0.0
PROJECT_BRIEF          = "High-performance C++ utility library"

INPUT                  = ../include
RECURSIVE              = YES
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = NO
EXTRACT_STATIC         = YES

GENERATE_HTML          = YES
GENERATE_LATEX         = NO
GENERATE_MAN           = YES
GENERATE_RTF           = NO
GENERATE_XML           = YES

HTML_OUTPUT            = html
HTML_HEADER            = header.html
HTML_FOOTER            = footer.html
HTML_STYLESHEET        = doxygen-awesome.css

HAVE_DOT               = YES
DOT_GRAPH_MAX_NODES    = 100
DOT_IMAGE_FORMAT       = svg
```

### 1.2 文档主题
使用现代化的文档主题：

```css
/* doxygen-awesome.css */
:root {
    --primary-color: #2c3e50;
    --secondary-color: #3498db;
    --text-color: #333;
    --background-color: #f8f9fa;
    --border-color: #e9ecef;
}

/* 自定义样式 */
.doxygen-awesome-theme {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    line-height: 1.6;
}
```

## 2. 文档结构规范

### 2.1 主页文档
创建项目主要信息页面：

```cpp
/**
 * @mainpage Leee C++ Library
 *
 * @section intro_sec Introduction
 *
 * Leee is a high-performance C++ utility library that provides:
 * - Efficient data structures (ByteArray, containers)
 * - High-performance algorithms
 * - Robust error handling
 * - Comprehensive logging facilities
 *
 * @section features_sec Key Features
 *
 * - **Zero-cost abstractions**: No runtime overhead for unused features
 * - **Exception safety**: Comprehensive exception handling
 * - **Thread safety**: Concurrent operations support
 * - **Memory efficiency**: Optimized memory usage
 *
 * @section usage_sec Basic Usage
 *
 * @code{.cpp}
 * #include <leee/bytearray.h>
 * #include <leee/logger.h>
 *
 * int main() {
 *     leee::init_logger();
 *     leee::ByteArray data("Hello, Leee!");
 *     LOG_INFO << "Data: " << data.str();
 *     return 0;
 * }
 * @endcode
 *
 * @section modules_sec Modules
 *
 * - @ref core "Core Module"
 * - @ref containers "Container Module"
 * - @ref algorithms "Algorithm Module"
 * - @ref services "Service Module"
 *
 * @section license_sec License
 *
 * This project is licensed under the MIT License - see the LICENSE file for details.
 */
```

### 2.2 模块文档
为每个模块创建详细文档：

```cpp
/**
 * @defgroup core Core Module
 * @brief 核心功能模块
 *
 * 核心模块提供基础功能和工具类。
 *
 * @section core_overview Overview
 *
 * 核心模块包含以下组件：
 * - 内存管理
 * - 线程同步
 * - 时间处理
 * - 基础工具
 *
 * @section core_usage Usage
 *
 * @code{.cpp}
 * #include <leee/core/memory.h>
 * #include <leee/core/threading.h>
 *
 * // 使用内存池
 * auto pool = leee::core::MemoryPool::instance();
 * void* ptr = pool->allocate(1024);
 * // 使用ptr
 * pool->deallocate(ptr);
 * @endcode
 */

/**
 * @defgroup containers Container Module
 * @brief 容器类模块
 *
 * 提供高效的容器实现。
 */
```

## 3. 函数和类文档

### 3.1 函数文档模板
完整的函数文档应包含：

```cpp
/**
 * @brief 函数的简要描述
 *
 * 函数的详细描述，包括功能、使用场景等。
 *
 * @param[in] input_param 输入参数描述
 * @param[out] output_param 输出参数描述
 * @param[in,out] inout_param 输入输出参数描述
 *
 * @return 返回值描述
 *
 * @throws ExceptionType 可能抛出的异常类型及条件
 *
 * @note 重要的注意事项
 * @warning 警告信息
 * @see 相关函数或类的引用
 *
 * @code{.cpp}
 * // 使用示例
 * int result = function_name(param);
 * @endcode
 */
ReturnType function_name(ParameterType input_param,
                        OutputType& output_param);
```

### 3.2 类文档模板
完整的类文档应包含：

```cpp
/**
 * @brief 类的简要描述
 *
 * 类的详细描述，包括设计意图、主要功能等。
 *
 * @section class_usage Usage
 *
 * @code{.cpp}
 * // 使用示例
 * ClassName instance;
 * instance.method();
 * @endcode
 *
 * @section class_design Design Notes
 *
 * 设计考虑和实现细节。
 *
 * @note 重要的实现注意事项
 * @warning 使用时的警告
 * @see 相关类
 */
class ClassName {
public:
    /**
     * @brief 构造函数
     * @param param 参数描述
     * @throws ExceptionType 构造失败时抛出的异常
     */
    explicit ClassName(ParameterType param);

    /**
     * @brief 主要方法
     * @return 操作结果
     */
    ReturnType method();

private:
    /// 私有成员变量描述
    MemberType member_;
};
```

### 3.3 枚举和常量文档
为枚举和常量提供文档：

```cpp
/**
 * @brief 用户状态枚举
 */
enum class UserStatus {
    ACTIVE,      ///< 用户处于活跃状态
    INACTIVE,    ///< 用户处于非活跃状态
    SUSPENDED,   ///< 用户被暂停
    DELETED      ///< 用户已被删除
};

/**
 * @brief 默认缓冲区大小
 *
 * 用于网络操作的默认缓冲区大小。
 * 该值经过性能测试优化。
 */
const size_t DEFAULT_BUFFER_SIZE = 8192;

/**
 * @brief 最大并发连接数
 *
 * 系统支持的最大并发连接数。
 * 超过此限制将拒绝新连接。
 */
const int MAX_CONNECTIONS = 1000;
```

## 4. 代码示例规范

### 4.1 基本示例
提供清晰的代码示例：

```cpp
/**
 * @brief 创建和使用ByteArray
 *
 * 演示ByteArray的基本用法。
 *
 * @code{.cpp}
 * #include <leee/bytearray.h>
 *
 * int main() {
 *     // 创建ByteArray
 *     leee::ByteArray data("Hello, World!");
 *
 *     // 基本操作
 *     std::cout << "Size: " << data.size() << std::endl;
 *     std::cout << "Content: " << data.str() << std::endl;
 *
 *     // 修改内容
 *     data += " Welcome to Leee!";
 *     data.toUpper();
 *
 *     std::cout << "Modified: " << data.str() << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
```

### 4.2 高级示例
提供复杂用法的示例：

```cpp
/**
 * @brief 多线程文件处理示例
 *
 * 演示如何在多线程环境中安全地处理文件。
 *
 * @code{.cpp}
 * #include <leee/bytearray.h>
 * #include <leee/logger.h>
 * #include <thread>
 * #include <vector>
 *
 * void process_file(const std::string& file_path) {
 *     try {
 *         // 读取文件内容
 *         auto content = leee::ByteArray::loadFromFile(file_path);
 *
 *         // 处理内容
 *         content.trim();
 *         content.toLower();
 *
 *         // 保存处理结果
 *         std::string output_path = file_path + ".processed";
 *         content.saveToFile(output_path);
 *
 *         LOG_INFO << "Processed file: " << file_path;
 *
 *     } catch (const std::exception& e) {
 *         LOG_ERROR << "Failed to process file " << file_path
 *                   << ": " << e.what();
 *     }
 * }
 *
 * int main() {
 *     leee::init_logger();
 *
 *     std::vector<std::string> files = {
 *         "data1.txt", "data2.txt", "data3.txt"
 *     };
 *
 *     std::vector<std::thread> threads;
 *     for (const auto& file : files) {
 *         threads.emplace_back(process_file, file);
 *     }
 *
 *     for (auto& thread : threads) {
 *         thread.join();
 *     }
 *
 *     LOG_INFO << "All files processed successfully";
 *     return 0;
 * }
 * @endcode
 */
```

## 5. 文档维护

### 5.1 文档更新流程
- 代码变更时同步更新文档
- 定期review文档的准确性和完整性
- 使用CI检查文档生成是否正常

### 5.2 文档质量检查
```yaml
# .github/workflows/docs.yml
name: Documentation
on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  docs:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v2

    - name: Install Doxygen
      run: sudo apt-get install -y doxygen graphviz

    - name: Generate Documentation
      run: |
        cd docs
        doxygen Doxyfile

    - name: Check Documentation
      run: |
        if [ ! -f html/index.html ]; then
          echo "Documentation generation failed"
          exit 1
        fi

    - name: Deploy Documentation
      if: github.ref == 'refs/heads/main'
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: ./docs/html
```

### 5.3 文档一致性检查
- 检查所有公共API都有文档
- 验证代码示例可以正常编译
- 确保函数签名与文档一致

## 6. 文档发布

### 6.1 在线文档
发布到GitHub Pages：

```yaml
# docs/index.html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Leee Documentation</title>
    <link rel="stylesheet" href="doxygen-awesome.css">
</head>
<body>
    <div class="container">
        <header>
            <h1>Leee C++ Library</h1>
            <p>High-performance C++ utility library</p>
        </header>

        <nav>
            <ul>
                <li><a href="html/index.html">API Reference</a></li>
                <li><a href="examples/">Examples</a></li>
                <li><a href="tutorials/">Tutorials</a></li>
            </ul>
        </nav>

        <main>
            <!-- 文档内容 -->
        </main>
    </div>
</body>
</html>
```

### 6.2 包文档
在发布包中包含文档：

```cmake
# CMakeLists.txt
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/html
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT documentation)

# 生成文档目标
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()
```

## 7. 最佳实践

### 7.1 文档编写原则
- **准确性**: 文档必须与代码实现一致
- **完整性**: 覆盖所有重要功能和用法
- **清晰性**: 使用简单明了的语言
- **实用性**: 提供有价值的示例和指导

### 7.2 文档维护建议
- 新功能开发时同步编写文档
- 定期review和更新现有文档
- 收集用户反馈改进文档
- 使用工具自动化文档检查

### 7.3 文档工具链
```bash
# 文档生成脚本
#!/bin/bash

# 生成API文档
doxygen Doxyfile

# 检查文档完整性
python3 scripts/check_docs.py

# 生成示例文档
python3 scripts/generate_examples.py

# 部署文档
python3 scripts/deploy_docs.py
```

---

*完善的API文档是优秀库的重要标志。*
