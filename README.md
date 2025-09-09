# Leee - AI驱动的现代化C++工具库

[![License](https://img.shields.io/badge/License-Unlicense-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Header-Only](https://img.shields.io/badge/Header--Only-Yes-green.svg)]()
[![AI-Driven](https://img.shields.io/badge/AI--Driven-Yes-blue.svg)]()

Leee是一个**AI驱动的现代化C++工具库**，采用**AI Agent + Header-Only**的创新开发模式，提供高效、智能化、跨平台的基础功能组件。

## 🤖 AI驱动开发理念

Leee项目开创性地采用**AI Agent主导开发**的新模式：

- 🎯 **AI Agent优先**: AI Agent负责代码生成、测试编写和优化
- 🚀 **智能协作**: 人机协同，优势互补
- 📊 **数据驱动**: 基于反馈的持续优化
- 🔄 **敏捷迭代**: 快速响应需求变化

## ✨ 核心特性

### 🤖 AI驱动特性
- 🧠 **智能代码生成**: AI Agent自动生成高质量C++代码
- 🧪 **自动化测试**: AI生成全面的测试用例和场景
- 📊 **性能优化**: AI驱动的代码优化和性能调优
- 🔍 **智能审查**: AI辅助的代码质量检查和安全扫描

### 🏗️ 架构特性
- 🚀 **Header-Only设计**: 无需编译安装，包含即可使用
- 🔧 **跨平台兼容**: 支持Windows、Linux、macOS等主流平台
- 📦 **零依赖**: 核心功能不依赖外部库，可选第三方库header-only
- 🎯 **现代C++**: 充分利用C++17/20特性
- 🏗️ **模块化架构**: 高内聚低耦合的设计
- 🧪 **完整测试**: 全面的单元测试和集成测试

## 📚 主要组件

### 核心模块

| 模块 | 描述 | 状态 |
|------|------|------|
| **ByteArray** | 增强的字符串类，Qt QByteArray风格 | ✅ 稳定 |
| **Logger** | 基于plog的高性能日志系统 | ✅ 稳定 |
| **Core Utilities** | 基础工具函数和类型定义 | 🚧 开发中 |
| **Network** | 异步网络编程组件 | 🚧 开发中 |
| **Algorithm** | 通用算法和数据结构 | 🚧 开发中 |

### 第三方库集成

Leee集成了以下优秀的header-only第三方库：

| 库名 | 版本 | 用途 |
|------|------|------|
| [Asio](header-only/asio/) | 1.28.0 | 异步I/O和网络编程 |
| [nlohmann/json](header-only/json/) | 3.11.2 | JSON解析和序列化 |
| [fmt](header-only/fmt/) | 10.1.1 | 现代化字符串格式化 |
| [plog](header-only/plog/) | 1.1.9 | 日志记录框架 |
| [Eigen](header-only/eigen/) | 3.4.0 | 线性代数计算 |
| [doctest](header-only/doctest/) | 2.4.11 | 单元测试框架 |
| [httplib](header-only/httplib/) | 0.14.1 | HTTP客户端/服务器 |
| [WebSocket++](header-only/websocketpp/) | 0.8.2 | WebSocket协议 |

## 🚀 快速开始

### 要求

- C++17兼容的编译器 (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.16+

### 包含到项目中

```cpp
// 包含头文件
#include <bytearray.h>  // 字符串工具
#include <logger.h>     // 日志系统

// 使用示例
int main() {
    // 字符串操作
    leee::ByteArray text("Hello Leee!");
    text.toUpper();  // 转换为大写

    // 日志记录
    LOG_INFO << "应用程序启动成功";

    return 0;
}
```

### CMake集成

```cmake
# 添加Leee到项目
add_subdirectory(path/to/leee)

# 或者包含第三方库
include(path/to/leee/header-only/third_party.cmake)

# 链接库
target_link_libraries(your_target PRIVATE leee)
```

## 📖 文档

- [API文档](doc/) - 详细的API参考文档
- [设计规范](design-specs/) - 架构设计和编码规范
- [示例代码](examples/) - 实际使用示例
- [测试报告](test/) - 完整的测试套件

## 🏗️ 构建和测试

```bash
# 克隆项目
git clone https://github.com/your-repo/leee.git
cd leee

# 创建构建目录
mkdir build && cd build

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Release

# 构建
make -j$(nproc)

# 运行测试
ctest --output-on-failure

# 生成文档
make docs
```

## 🎯 AI驱动Header-Only设计理念

Leee开创性地融合**AI技术和Header-Only设计理念**：

### 🤖 AI + Header-Only的独特优势

#### 传统Header-Only优势
1. **简化依赖管理**: 无需处理复杂的库依赖关系
2. **提升编译效率**: 模板实例化和内联优化
3. **增强可移植性**: 减少二进制兼容性问题
4. **简化分发**: 单头文件即可使用
5. **更好的优化**: 编译器可以进行跨模块优化

#### AI增强优势
6. **智能代码生成**: AI Agent快速生成高质量Header-Only组件
7. **自动化测试**: AI生成全面的测试用例和边界条件
8. **性能优化**: AI驱动的算法优化和内存管理
9. **文档生成**: AI自动生成详细的使用文档和示例
10. **质量保证**: AI辅助的代码审查和错误检测

### 🎯 开发模式创新

**AI Agent主导，人类指导**
- 🤖 **AI Agent**: 负责代码编写、测试生成、性能优化
- 👥 **人类开发者**: 提供架构设计、需求分析、质量把关
- 🔄 **协作模式**: 人机协同，优势互补，持续改进

### 📋 第三方库选择标准

- ✅ **许可证兼容**: MIT、BSD、Apache等宽松许可证
- ✅ **活跃维护**: 最近一年内有更新
- ✅ **跨平台支持**: 支持主流操作系统
- ✅ **Header-Only**: 纯头文件实现，无需编译
- ✅ **测试覆盖**: 完善的测试套件
- ✅ **AI友好**: 易于AI理解和优化的API设计

## 🤝 贡献

欢迎贡献代码！Leee采用**AI驱动的协作模式**，支持多种贡献方式：

### 🤖 AI辅助贡献

**推荐方式**: 使用AI工具加速开发流程

1. **AI生成代码**: 使用AI工具生成组件代码
2. **AI生成测试**: 让AI创建相应的测试用例
3. **AI优化代码**: 使用AI进行性能优化
4. **人类审查**: 人工审核AI生成的内容质量

### 👥 传统贡献流程

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

### 📋 贡献要求

- 遵循 [编码规范](design-specs/coding-standards/)
- 添加相应的单元测试 (可由AI生成)
- 更新相关文档 (AI可辅助生成)
- 通过所有测试用例
- 标注AI生成内容的贡献者信息

## 📄 许可证

本项目采用 [Unlicense](LICENSE) 许可证 - 详见 [LICENSE](LICENSE) 文件。

**第三方库许可证**:
- 各第三方库保持其原始许可证
- 所有许可证都经过兼容性验证
- 详见各库的 `metadata.json` 文件

## 👥 作者

**Sail** - *项目维护者*  
📧 sailcpu@gmail.com  
🏢 个人项目

## 🙏 致谢

- 感谢所有贡献者的辛勤工作
- 感谢第三方库的维护者提供优秀工具
- 感谢开源社区的持续支持

## 📊 项目状态

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Test Coverage](https://img.shields.io/badge/coverage-85%25-yellow.svg)]()
[![Documentation](https://img.shields.io/badge/docs-generated-blue.svg)]()

---

*"AI赋能开发，简单即是美 - Leee致力于用AI驱动的方式提供最强大的功能"*

## 🙌 致谢

- **AI技术的贡献者**: 感谢AI技术的进步使这一创新开发模式成为可能
- **开源社区**: 感谢所有第三方库的维护者
- **贡献者**: 感谢所有为项目贡献代码和建议的开发者

⭐ 如果这个项目对你有帮助，请给它一个星标！

---

*🚀 本文档部分内容由AI Agent辅助生成，体现了Leee的AI驱动开发理念*
