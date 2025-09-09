# 单元测试规范

## 概述

单元测试是确保代码质量和可靠性的基础。本文档定义了Leee项目的单元测试规范、实践和工具使用指南。

## 1. 测试原则

### 1.1 FIRST原则
- **Fast (快速)**: 测试执行速度快
- **Independent (独立)**: 测试间相互独立
- **Repeatable (可重复)**: 结果一致可重复
- **Self-Validating (自验证)**: 自动验证结果
- **Timely (及时)**: 与代码同步编写

### 1.2 测试覆盖目标
- **语句覆盖率**: > 80%
- **分支覆盖率**: > 75%
- **函数覆盖率**: > 90%
- **条件覆盖率**: > 70%

## 2. 测试框架

### 2.1 Doctest框架
使用doctest作为主要测试框架：

```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <leee/bytearray.h>

// 测试用例
TEST_CASE("ByteArray - Basic Operations") {
    leee::ByteArray ba("test");

    SUBCASE("Size Check") {
        CHECK(ba.size() == 4);
    }

    SUBCASE("Empty Check") {
        CHECK_FALSE(ba.isEmpty());
    }
}
```

### 2.2 测试宏使用
- `TEST_CASE`: 定义测试用例
- `SUBCASE`: 定义子测试用例
- `CHECK`: 基本断言
- `REQUIRE`: 强断言（失败时终止测试）
- `CHECK_THROWS`: 检查异常抛出

## 3. 测试组织结构

### 3.1 文件命名规范
- 测试文件以`test_`开头
- 与被测模块同名
- 放在`test/`目录下

```
test/
├── test_bytearray.cpp      # ByteArray测试
├── test_logger.cpp         # Logger测试
├── test_memory.cpp         # 内存管理测试
└── test_algorithms.cpp     # 算法测试
```

### 3.2 测试类组织
按功能模块组织测试：

```cpp
// test_bytearray.cpp
TEST_CASE("ByteArray - Construction") {
    // 构造相关测试
}

TEST_CASE("ByteArray - String Operations") {
    // 字符串操作测试
}

TEST_CASE("ByteArray - Memory Operations") {
    // 内存操作测试
}

TEST_CASE("ByteArray - File Operations") {
    // 文件操作测试
}
```

## 4. 测试用例编写

### 4.1 测试用例结构
每个测试用例应包含：
- **Arrange**: 设置测试前提
- **Act**: 执行被测操作
- **Assert**: 验证结果

```cpp
TEST_CASE("UserManager - Add User") {
    // Arrange - 准备测试数据
    UserManager manager;
    User test_user{"john", "john@example.com"};

    // Act - 执行操作
    bool result = manager.add_user(test_user);

    // Assert - 验证结果
    CHECK(result == true);
    CHECK(manager.user_count() == 1);
    CHECK(manager.find_user("john") != nullptr);
}
```

### 4.2 边界条件测试
测试边界条件和异常情况：

```cpp
TEST_CASE("ByteArray - Boundary Conditions") {
    SUBCASE("Empty String") {
        leee::ByteArray empty;
        CHECK(empty.size() == 0);
        CHECK(empty.isEmpty());
    }

    SUBCASE("Very Long String") {
        std::string long_str(1000000, 'x');
        leee::ByteArray ba(long_str);
        CHECK(ba.size() == 1000000);
    }

    SUBCASE("Null Characters") {
        std::string with_null("hello\0world", 11);
        leee::ByteArray ba(with_null);
        CHECK(ba.size() == 11);
    }
}
```

### 4.3 异常测试
测试异常抛出情况：

```cpp
TEST_CASE("FileSystem - Exception Handling") {
    FileSystem fs;

    SUBCASE("Invalid Path") {
        CHECK_THROWS_AS(fs.open_file(""), std::invalid_argument);
    }

    SUBCASE("Permission Denied") {
        CHECK_THROWS_AS(fs.open_file("/root/system.conf"),
                       std::runtime_error);
    }
}
```

## 5. 模拟对象和依赖注入

### 5.1 模拟对象使用
使用Google Mock创建模拟对象：

```cpp
class MockDatabase : public IDatabase {
public:
    MOCK_METHOD(bool, connect, (), (override));
    MOCK_METHOD(bool, disconnect, (), (override));
    MOCK_METHOD(User, find_user, (int), (override));
};

TEST_CASE("UserService - Find User") {
    auto mock_db = std::make_shared<MockDatabase>();
    UserService service(mock_db);

    // 设置期望
    EXPECT_CALL(*mock_db, find_user(123))
        .WillOnce(Return(User{123, "john"}));

    // 执行测试
    auto user = service.find_user(123);
    CHECK(user.id == 123);
    CHECK(user.name == "john");
}
```

### 5.2 依赖注入
设计类以支持依赖注入：

```cpp
class UserService {
public:
    // 构造函数注入
    UserService(std::shared_ptr<IDatabase> db)
        : database_(db) {}

    // 方法注入
    void set_cache(std::shared_ptr<ICache> cache) {
        cache_ = cache;
    }

private:
    std::shared_ptr<IDatabase> database_;
    std::shared_ptr<ICache> cache_;
};
```

## 6. 测试数据管理

### 6.1 测试数据准备
使用测试夹具管理测试数据：

```cpp
class DatabaseTestFixture {
protected:
    void SetUp() override {
        // 准备测试数据库
        db_.connect("test_database");
        db_.clear_all_data();
    }

    void TearDown() override {
        // 清理测试数据
        db_.disconnect();
    }

    TestDatabase db_;
};

TEST_CASE_FIXTURE(DatabaseTestFixture, "Database - CRUD Operations") {
    // 测试使用已准备的数据库
    User user{"test", "test@example.com"};
    CHECK(db_.insert_user(user));
    CHECK(db_.find_user("test") != nullptr);
}
```

### 6.2 测试数据隔离
确保测试间数据隔离：

```cpp
class UserServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试使用独立的数据库
        db_name_ = "test_db_" + std::to_string(rand());
        service_ = std::make_unique<UserService>(db_name_);
    }

    void TearDown() override {
        // 清理测试数据库
        service_.reset();
        cleanup_database(db_name_);
    }

    std::unique_ptr<UserService> service_;
    std::string db_name_;
};
```

## 7. 性能测试

### 7.1 基准测试
使用Google Benchmark进行性能测试：

```cpp
static void BM_StringConcatenation(benchmark::State& state) {
    std::string base = "test";
    for (auto _ : state) {
        std::string result = base + "suffix";
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_StringConcatenation);

static void BM_ByteArrayConcatenation(benchmark::State& state) {
    leee::ByteArray base("test");
    for (auto _ : state) {
        leee::ByteArray result = base + "suffix";
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ByteArrayConcatenation);
```

### 7.2 内存测试
测试内存使用情况：

```cpp
TEST_CASE("Memory Usage - ByteArray Operations") {
    MemoryTracker tracker;

    SUBCASE("Small Allocations") {
        tracker.start_tracking();
        for (int i = 0; i < 1000; ++i) {
            leee::ByteArray ba(std::to_string(i));
        }
        tracker.stop_tracking();

        CHECK(tracker.peak_memory() < 1024 * 1024); // 小于1MB
    }
}
```

## 8. 测试执行和报告

### 8.1 测试运行
使用CTest运行测试：

```bash
# 运行所有测试
ctest

# 运行特定测试
ctest -R "ByteArray"

# 运行测试并显示详细信息
ctest --output-on-failure

# 运行性能测试
ctest -R "benchmark"
```

### 8.2 覆盖率分析
使用gcov生成覆盖率报告：

```bash
# 编译时启用覆盖率
cmake -DCMAKE_BUILD_TYPE=Coverage ..

# 运行测试
make test

# 生成覆盖率报告
gcovr -r . --html --html-details -o coverage.html
```

### 8.3 持续集成
在CI/CD中集成测试：

```yaml
# .github/workflows/ci.yml
name: CI
on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v2
    - name: Build
      run: |
        mkdir build && cd build
        cmake .. -DCMAKE_BUILD_TYPE=Debug
        make -j$(nproc)

    - name: Test
      run: |
        cd build
        ctest --output-on-failure

    - name: Coverage
      run: |
        cd build
        gcovr -r .. --xml -o coverage.xml
```

## 9. 测试最佳实践

### 9.1 测试命名规范
- 测试方法名描述被测行为
- 使用Given_When_Then格式

```cpp
TEST_CASE("Given_ValidUser_When_Login_Then_Success") {
    // 测试实现
}

TEST_CASE("Given_InvalidPassword_When_Login_Then_Failure") {
    // 测试实现
}
```

### 9.2 测试代码质量
- 测试代码应与产品代码同等质量
- 使用DRY原则避免重复
- 保持测试的可读性和维护性

### 9.3 失败测试处理
- 快速定位失败原因
- 隔离失败的测试
- 记录已知问题

---

*全面的单元测试是代码质量的保障。*
