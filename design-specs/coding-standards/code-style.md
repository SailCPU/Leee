# 代码风格规范

## 概述

统一的代码风格确保代码的可读性和一致性。本文档定义了Leee项目的代码格式化、缩进和布局规范。

## 1. 基本格式规范

### 1.1 缩进
- 使用4个空格进行缩进
- 不使用制表符
- 换行保持缩进对齐

```cpp
// 正确
if (condition) {
    do_something();
    if (nested_condition) {
        do_nested_thing();
    }
}

// 错误 - 使用制表符
if (condition) {
	do_something();  // 制表符缩进
}
```

### 1.2 行长度
- 单行最大长度：100字符
- 长行应适当换行
- 注释行可超过限制，但应合理

```cpp
// 正确 - 行长度适中
bool is_user_authenticated = authenticate_user(username, password);

// 正确 - 长行适当换行
bool result = process_user_data(user_id, data,
                               options, callback);

// 错误 - 单行过长
bool result = process_user_data(user_id, data, options, callback, additional_parameter, another_parameter);
```

### 1.3 空行
- 函数之间空一行
- 类方法之间空一行
- 逻辑块之间适当空行
- 避免连续多个空行

```cpp
class UserManager {
public:
    UserManager();

    bool authenticate_user(const std::string& username,
                          const std::string& password);

    void logout_user(int user_id);

private:
    std::vector<User> users_;
};

bool UserManager::authenticate_user(const std::string& username,
                                   const std::string& password) {
    // 验证输入参数
    if (username.empty() || password.empty()) {
        return false;
    }

    // 查找用户
    auto user = find_user_by_username(username);
    if (!user) {
        return false;
    }

    // 验证密码
    return validate_password(password, user->password_hash);
}
```

## 2. 括号和语句结构

### 2.1 大括号风格
- 使用K&R风格（左大括号与控制语句同行）
- 函数定义的大括号单独一行

```cpp
// 函数定义
void function_name() {
    if (condition) {
        do_something();
    } else {
        do_other_thing();
    }
}

// 类定义
class MyClass {
public:
    MyClass();
    ~MyClass();

private:
    int member_;
};
```

### 2.2 条件语句
- 条件语句始终使用大括号
- 即使是单行语句

```cpp
// 正确
if (user_count > 0) {
    process_users();
}

// 错误 - 缺少大括号
if (user_count > 0)
    process_users();
```

### 2.3 循环语句
- for循环格式统一
- 循环变量声明在循环内部

```cpp
// 正确
for (int i = 0; i < user_count; ++i) {
    process_user(i);
}

// 范围for循环
for (const auto& user : users) {
    process_user(user);
}
```

## 3. 空格和对齐

### 3.1 二元运算符
- 二元运算符前后各一个空格
- 一元运算符不加空格

```cpp
// 正确
int result = a + b;
bool condition = (x > 0) && (y < 10);
int value = -x;

// 错误
int result=a+b;
bool condition = (x>0)&&(y<10);
```

### 3.2 逗号和分号
- 逗号后加空格
- 分号前不加空格

```cpp
// 正确
function_call(arg1, arg2, arg3);
for (int i = 0; i < 10; ++i) {
    // ...
}
```

### 3.3 指针和引用
- 指针和引用符号靠近类型

```cpp
// 正确
int* pointer = nullptr;
const std::string& reference = get_string();
void process_data(const Data* data);

// 不推荐（但可接受）
int *pointer = nullptr;
const std::string &reference = get_string();
```

## 4. 头文件和包含

### 4.1 包含顺序
- 标准库头文件
- 第三方库头文件
- 项目内部头文件

```cpp
// 正确的包含顺序
#include <iostream>
#include <vector>
#include <memory>

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include "user_manager.h"
#include "database_connection.h"
```

### 4.2 头文件保护符
- 使用#pragma once
- 或者使用宏保护符

```cpp
// 推荐方式
#pragma once

// 或者传统方式
#ifndef LEE_USER_MANAGER_H
#define LEE_USER_MANAGER_H

// 头文件内容

#endif // LEE_USER_MANAGER_H
```

## 5. 注释规范

### 5.1 文件头注释
- 包含文件描述、作者、日期等信息

```cpp
//
// UserManager.h
// 用户管理类定义
//
// 作者: 张三
// 创建日期: 2024-01-01
// 修改日期: 2024-09-09
//

#pragma once
```

### 5.2 函数注释
- 使用Doxygen格式
- 描述功能、参数、返回值

```cpp
/**
 * @brief 用户认证函数
 * @param username 用户名
 * @param password 密码
 * @return 认证是否成功
 */
bool authenticate_user(const std::string& username,
                      const std::string& password);
```

### 5.3 代码内联注释
- 解释复杂逻辑
- 标记TODO和FIXME

```cpp
// 计算用户积分
int calculate_score(const User& user) {
    int base_score = 100;

    // 根据用户活跃度调整积分
    if (user.activity_level > 5) {
        base_score += 50;
    }

    // TODO: 考虑用户等级影响
    // FIXME: 处理负数积分的情况

    return base_score;
}
```

## 6. 模板和泛型

### 6.1 模板声明格式
- template关键字单独一行
- 参数对齐

```cpp
template <typename T>
class Container {
public:
    void add(const T& item);
    T get(size_t index) const;
};

template <typename KeyType, typename ValueType>
class Map {
    // ...
};
```

### 6.2 模板特化
- 特化声明清晰标识

```cpp
// 主模板
template <typename T>
class Serializer {
public:
    static std::string serialize(const T& value);
};

// 特化版本
template <>
class Serializer<int> {
public:
    static std::string serialize(int value) {
        return std::to_string(value);
    }
};
```

## 7. 异常处理

### 7.1 异常规范
- 明确声明可能抛出的异常
- 使用标准异常类型

```cpp
/**
 * @brief 加载配置文件
 * @param file_path 配置文件路径
 * @throw std::runtime_error 文件不存在或格式错误
 * @throw std::invalid_argument 无效参数
 */
void load_config(const std::string& file_path) {
    if (file_path.empty()) {
        throw std::invalid_argument("File path cannot be empty");
    }

    // 加载配置逻辑
    // ...
}
```

## 8. 代码格式化工具

### 8.1 推荐工具
- clang-format: 自动代码格式化
- clang-tidy: 代码质量检查
- cpplint: Google风格检查

### 8.2 配置示例
.clang-format配置文件示例：
```yaml
Language: Cpp
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 100
```

## 9. 代码审查清单

### 9.1 格式检查要点
- [ ] 缩进是否正确（4个空格）
- [ ] 行长度是否超过限制
- [ ] 大括号风格是否一致
- [ ] 空格使用是否规范
- [ ] 注释是否完整

### 9.2 质量检查要点
- [ ] 变量命名是否规范
- [ ] 函数复杂度是否适中
- [ ] 错误处理是否完善
- [ ] 内存管理是否安全

---

*良好的代码风格是高质量代码的基础。*
