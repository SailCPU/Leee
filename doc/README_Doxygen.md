# Leee 项目 Doxygen 文档系统

本文档介绍如何使用Doxygen为Leee项目生成API文档。

## 目录结构

```
Leee/
├── Doxyfile                    # Doxygen配置文件
├── generate_docs.sh           # 文档生成脚本
├── docs/                       # 生成的文档输出目录
├── include/                    # 头文件目录
│   ├── bytearray.h            # ByteArray模块
│   └── logger.h               # Logger模块
└── doc/                        # 文档源文件目录
    ├── mainpage.dox           # 主页文档
    ├── bytearray.dox          # ByteArray详细文档
    ├── logger.dox             # Logger详细文档
    └── README_Doxygen.md      # 本文档
```

## 快速开始

### 1. 生成文档

运行以下命令生成完整的API文档：

```bash
# 在项目根目录执行
./generate_docs.sh
```

或者直接使用doxygen：

```bash
doxygen Doxyfile
```

### 2. 查看文档

文档生成后，打开浏览器访问：
- `docs/index.html` - 主页
- 完整的HTML文档将在 `docs/` 目录中生成

### 3. 清理文档

```bash
./generate_docs.sh --clean
```

## 文档特性

### 支持的功能

- ✅ **完整的API文档**: 自动从代码注释生成
- ✅ **类图和继承图**: 使用Graphviz生成
- ✅ **调用图和被调用图**: 显示函数关系
- ✅ **文件依赖图**: 显示文件间的依赖关系
- ✅ **模块分组**: 按功能模块组织文档
- ✅ **搜索功能**: 内置搜索功能
- ✅ **多种输出格式**: HTML、PDF（可选）
- ✅ **自定义主题**: 支持自定义样式

### 文档内容

1. **主页 (mainpage.dox)**
   - 项目概述
   - 主要特性
   - 快速开始指南
   - API参考

2. **模块文档**
   - **ByteArray**: 字符串工具模块详解
   - **Logger**: 日志记录模块详解

3. **API文档**
   - 自动从头文件注释生成
   - 包含所有类、函数、变量的详细说明
   - 参数说明、返回值、使用示例

## 配置说明

### Doxyfile 配置

主要的配置项：

```doxygen
# 项目信息
PROJECT_NAME           = "Leee"
PROJECT_NUMBER         = "1.0.0"
PROJECT_BRIEF          = "轻量级C++工具库"

# 输入输出
INPUT                  = ./include ./doc
OUTPUT_DIRECTORY       = ./docs
HTML_OUTPUT            = .

# 功能开关
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = YES
EXTRACT_STATIC         = YES

# 图表生成
HAVE_DOT               = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
```

### 注释规范

#### 文件注释

```cpp
/**
 * @file filename.h
 * @brief 文件功能简述
 * @details 详细功能描述
 *
 * @author 作者
 * @version 版本号
 * @date 日期
 */
```

#### 类注释

```cpp
/**
 * @brief 类功能简述
 *
 * 详细的功能描述，包括：
 * - 主要特性
 * - 使用方法
 * - 注意事项
 *
 * @section usage 使用示例
 * @code
 * // 示例代码
 * @endcode
 *
 * @section features 特性
 * - 特性1
 * - 特性2
 *
 * @note 注意事项
 * @warning 警告信息
 * @sa 相关类或函数
 */
```

#### 函数注释

```cpp
/**
 * @brief 函数功能简述
 * @param param1 参数1说明
 * @param param2 参数2说明
 * @return 返回值说明
 * @throw 异常说明
 *
 * 详细的功能描述和使用示例
 */
```

#### 成员变量注释

```cpp
/** @brief 变量功能说明 */
int memberVariable;
```

## 高级功能

### 1. 条件编译

```cpp
#ifdef DOXYGEN
// 只在文档中显示的代码
#endif
```

### 2. 自定义命令

```cpp
/** @custom_command 自定义内容 */
```

### 3. 数学公式

```cpp
@f[
  \int_{-\infty}^{\infty} e^{-x^2} dx = \sqrt{\pi}
@f]
```

### 4. 引用和链接

```cpp
/** @see otherFunction 参考其他函数 */
/** @ref MyClass 链接到MyClass */
```

## 故障排除

### 常见问题

1. **文档生成失败**
   - 检查Doxyfile路径是否正确
   - 确认输入目录存在
   - 检查文件权限

2. **图表不显示**
   - 安装Graphviz：`sudo apt install graphviz`
   - 确认Doxyfile中`HAVE_DOT = YES`

3. **中文乱码**
   - 设置`DOXYFILE_ENCODING = UTF-8`
   - 确保源文件使用UTF-8编码

4. **缺少某些内容**
   - 检查注释格式是否符合doxygen规范
   - 确认相关文件在INPUT目录中

### 调试技巧

1. **查看警告信息**
   ```bash
   doxygen Doxyfile 2>&1 | tee doxygen_warnings.txt
   ```

2. **启用详细输出**
   ```bash
   doxygen Doxyfile -d 调试选项
   ```

3. **检查配置文件**
   ```bash
   doxygen -g  # 生成默认配置
   ```

## 最佳实践

### 1. 注释风格

- 使用`/** */`格式的注释
- 每个公共API都要有详细注释
- 使用标准的doxygen命令

### 2. 文档结构

- 按照功能分组组织文档
- 使用`@section`划分文档章节
- 提供丰富的示例代码

### 3. 维护更新

- 代码修改时同步更新注释
- 定期重新生成文档
- 检查文档的准确性

## 工具和资源

### 推荐工具

- **Doxygen**: 文档生成工具
- **Graphviz**: 图表生成工具
- **Doxygen Wizard**: GUI配置工具

### 相关链接

- [Doxygen官网](https://www.doxygen.nl/)
- [Doxygen手册](https://www.doxygen.nl/manual/)
- [Doxygen命令参考](https://www.doxygen.nl/manual/commands.html)

## 贡献指南

### 文档贡献

1. 遵循现有的注释规范
2. 为新功能编写详细文档
3. 更新相关的示例代码
4. 测试文档生成是否正常

### 代码贡献

1. 为所有公共API添加doxygen注释
2. 更新相关文档文件
3. 运行文档生成脚本验证
4. 提交前检查文档质量

---

**注意**: 本文档会随着项目的发展持续更新。如有问题或建议，请提交Issue。
