# Leee Unit Tests

Leee 项目的单元测试目录，使用 [doctest](https://github.com/doctest/doctest) 框架进行单元测试。

## 目录结构

```
test/
├── test_bytearray.cpp      # ByteArray 模块单元测试
├── CMakeLists.txt          # 测试构建配置
└── README.md              # 本文件
```

## 测试用例

### ByteArray 模块测试

包含以下测试用例：

1. **基本构造和初始化** - 测试构造函数、默认构造、字符串构造等
2. **字符串操作** - 测试子串、查找、替换、大小写转换等
3. **字符串修改** - 测试追加、插入、删除等修改操作
4. **字符串分割和连接** - 测试 split() 和 join() 操作
5. **查找操作** - 测试 indexOf()、lastIndexOf() 等查找功能
6. **类型转换** - 测试数字与字符串的相互转换
7. **文件操作** - 测试文件保存和加载功能
8. **路径操作** - 测试路径解析功能
9. **编码操作** - 测试十六进制编码解码
10. **比较操作** - 测试字符串比较功能
11. **工具函数** - 测试时间戳、填充等工具函数
12. **运算符重载** - 测试 +、+= 等运算符
13. **数据打包和解包** - 测试 pack() 和 unpack() 功能
14. **性能分析工具** - 测试 PerformanceUtil 类

## 编译和运行

### 方法一：直接编译

```bash
# 在 test 目录下
cd test
g++ -std=c++17 -I../include -I../header-only -o test_bytearray test_bytearray.cpp
./test_bytearray
```

### 方法二：使用 CMake

```bash
# 在 test 目录下
cd test
mkdir build && cd build
cmake ..
make
./test_bytearray
```

## 测试输出

测试运行时会显示：
- 测试用例数量
- 通过/失败的断言数量
- 详细的失败信息（如果有）

示例输出：
```
[doctest] doctest version is "2.4.12"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 14 | 14 passed | 0 failed | 0 skipped
[doctest] assertions: 93 | 93 passed | 0 failed |
[doctest] Status: SUCCESS!
```

## 测试选项

doctest 支持多种运行选项：

```bash
./test_bytearray --help              # 显示帮助信息
./test_bytearray -tc="ByteArray*"    # 运行特定测试用例
./test_bytearray -v                  # 详细输出
./test_bytearray -d                  # 显示测试持续时间
```

## 添加新测试

要为其他模块添加测试：

1. 创建新的测试文件 `test_<module>.cpp`
2. 在文件顶部包含必要的头文件
3. 使用 `TEST_CASE` 定义测试用例
4. 使用 `CHECK`、`REQUIRE` 等宏进行断言
5. 更新 `CMakeLists.txt` 添加新的可执行目标

## 注意事项

- 所有测试都是独立的，不会相互影响
- 测试文件会创建临时文件，请确保有适当的写入权限
- 日志模块会输出到控制台，这是正常行为
- 性能测试的结果可能因系统负载而异
