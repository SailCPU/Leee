# Leee ByteArray Module

Leee 项目的字节数组工具模块，提供丰富的字节数组操作功能，与日志模块保持一致的命名风格。

## 特性

- **Header-only**: 完全头文件实现，无需编译
- **一致的命名风格**: 与日志模块保持相同的命名规范
- **丰富的工具函数**: 提供常用的字符串处理方法
- **类型转换**: 支持各种数据类型的字符串转换
- **文件操作**: 读写文件功能
- **编码解码**: 十六进制编码
- **性能分析**: 内置性能分析工具
- **格式化支持**: 支持格式化字符串

## 快速开始

### 1. 包含头文件

```cpp
#include <bytearray.h>
```

### 2. 基本使用

```cpp
using leee::ByteArray;

// 创建字符串
ByteArray str("Hello World");

// 字符串操作
ByteArray left = str.left(5);      // "Hello"
ByteArray right = str.right(5);    // "World"
bool starts = str.startsWith("Hello"); // true

// 分割字符串
auto parts = str.split(' '); // ["Hello", "World"]
```

### 3. 类型转换

```cpp
ByteArray numStr = ByteArray::number(42);    // "42"
ByteArray hexStr = ByteArray::number(255, 16); // "0xff"

int value = numStr.toInt();      // 42
double d = ByteArray("3.14").toDouble(); // 3.14
```

### 4. 文件操作

```cpp
ByteArray content("Hello File!");
content.saveToFile("test.txt");

ByteArray loaded = ByteArray::loadFromFile("test.txt");
// loaded == "Hello File!"
```

## 主要功能

### 字符串操作

- `left(size_t len)`: 获取左子串
- `right(size_t len)`: 获取右子串
- `mid(size_t pos, size_t len)`: 获取中间子串
- `split(char delimiter)`: 按字符分割
- `split(const std::string& delimiter)`: 按字符串分割
- `replace(const std::string& oldStr, const std::string& newStr)`: 替换字符串
- `toLower()` / `toUpper()`: 大小写转换
- `trim()` / `trimLeft()` / `trimRight()`: 去除空白字符

### 类型转换

- `number()`: 将数字转换为字符串（支持不同进制）
- `toInt()`, `toLong()`, `toLongLong()`: 转换为整数
- `toUInt()`, `toULong()`, `toULongLong()`: 转换为无符号整数
- `toFloat()`, `toDouble()`: 转换为浮点数

### 编码解码

- `toHexString()`: 转换为十六进制字符串
- `fromHexString()`: 从十六进制字符串转换

### 文件操作

- `saveToFile(const std::string& path)`: 保存到文件
- `appendToFile(const std::string& path)`: 追加到文件
- `loadFromFile(const std::string& path)`: 从文件加载

### 实用工具

- `getCurrentTimeStampTicksMs()`: 获取当前时间戳（毫秒）
- `getEnvironmentVariable(const std::string& name)`: 获取环境变量
- `getFileName()`, `getDirectory()`, `getFileExtension()`: 路径处理
- `pack<T>()` / `unpack<T>()`: 数据打包/解包

## 高级用法

### 性能分析

```cpp
leee::PerformanceUtil perf;

perf.reset();
// 执行一些操作
perf.recordTimePoint();
// 执行更多操作
perf.recordTimePoint();

ByteArray result = perf.getReport();
LOG_INFO << "Performance: " << result.str();
```

### 字符串格式化

```cpp
// 使用格式化函数
ByteArray formatted = leee::formatString("Value: {}, Name: {}", 42, "test");
// formatted.str() == "Value: 42, Name: test"

// 直接构造
ByteArray str = ByteArray::formatString("Hello {}", "World");
```

### 路径操作

```cpp
ByteArray filePath("/home/user/documents/test.txt");

ByteArray fileName = ByteArray::getFileName(filePath.str());        // "test.txt"
ByteArray directory = ByteArray::getDirectory(filePath.str());      // "/home/user/documents/"
ByteArray extension = ByteArray::getFileExtension(filePath.str());  // ".txt"
```

### 数据序列化

```cpp
// 打包数据
int value = 42;
ByteArray packed = ByteArray::pack(value);

// 解包数据
int unpacked = ByteArray::unpack<int>(packed.c_str());
```

## 依赖

- C++17 或更高版本
- fmt 库 (已包含在 `header-only/fmt/` 中)
- nlohmann/json 库 (已包含在 `header-only/json/` 中)

## 编译选项

确保包含路径包含：
- Leee 项目头文件路径
- header-only 路径

### CMake 示例

```cmake
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/header-only
)
```

## 示例代码

查看项目中的示例代码了解更多用法。

## 注意事项

1. ByteArray 使用内部 std::string 存储数据
2. 所有方法都是线程安全的
3. 文件操作会自动处理二进制数据
4. 类型转换失败时会返回默认值，并可通过参数检查是否成功
5. 路径处理函数会自动适配 Windows 和 Linux 路径分隔符
6. 命名风格与日志模块保持一致：驼峰命名，动词开头