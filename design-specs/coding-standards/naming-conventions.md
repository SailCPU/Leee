# 命名规范

## 概述

统一的命名规范是确保代码可读性和一致性的基础。本文档定义了Leee项目中各类标识符的命名规则。

## 1. 通用命名原则

### 1.1 清晰性和描述性
- 使用有意义的名称，避免缩写（除非是广为人知的）
- 名称应该准确描述变量/函数/类的用途
- 避免使用单个字符作为变量名（除循环变量外）

```cpp
// 推荐
std::string user_name;
int calculate_total_amount();
class UserAuthenticationManager;

// 不推荐
std::string un;
int calc();
class UAM;
```

### 1.2 一致性
- 在整个项目中使用统一的命名风格
- 遵循现有代码的命名模式
- 避免混合不同的命名风格

## 2. C++命名规范

### 2.1 变量命名
- 使用蛇形命名法（snake_case）
- 局部变量使用小写字母开头
- 成员变量以下划线结尾

```cpp
// 局部变量
int user_count = 0;
std::string file_path = "/tmp/data.txt";

// 成员变量
class UserManager {
private:
    int user_count_;
    std::string config_file_path_;
    std::vector<User> users_;
};
```

### 2.2 函数命名
- 使用蛇形命名法
- 动词开头，描述函数执行的操作

```cpp
// 推荐
void process_user_data();
bool validate_user_input();
int calculate_total_score();

// 不推荐
void process();
bool check();
int calc();
```

### 2.3 类和结构体命名
- 使用帕斯卡命名法（PascalCase）
- 描述类的主要职责

```cpp
class UserAuthenticationManager;
class DataProcessingPipeline;
struct HttpRequestHeader;
```

### 2.4 命名空间命名
- 使用小写字母
- 简短且具有描述性

```cpp
namespace leee {
namespace network {
namespace http {
    // HTTP相关代码
}
}
}
```

### 2.5 枚举命名
- 枚举类型使用帕斯卡命名法
- 枚举值使用全大写蛇形命名法

```cpp
enum class UserStatus {
    ACTIVE,
    INACTIVE,
    SUSPENDED,
    DELETED
};

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE
};
```

### 2.6 常量命名
- 使用全大写蛇形命名法
- 在定义处添加注释说明

```cpp
// 最大用户数量
const int MAX_USER_COUNT = 1000;

// 默认超时时间（毫秒）
const int DEFAULT_TIMEOUT_MS = 5000;

// 数据库连接字符串
const std::string DATABASE_CONNECTION_STRING = "postgresql://localhost:5432/leee";
```

### 2.7 宏命名
- 使用全大写蛇形命名法
- 添加项目前缀避免冲突

```cpp
#define LEE_LOG_INFO(message) leee::logger.info() << message
#define LEE_CHECK(condition) assert(condition)
#define LEE_STRINGIFY(x) #x
```

## 3. 文件和目录命名

### 3.1 源文件命名
- 使用蛇形命名法
- 与主要类或模块名对应

```
user_manager.cpp
data_processor.cpp
http_client.cpp
```

### 3.2 头文件命名
- 与对应源文件同名
- 使用.h扩展名

```
user_manager.h
data_processor.h
http_client.h
```

### 3.3 目录命名
- 使用蛇形命名法
- 描述目录内容

```
include/
src/
test/
docs/
scripts/
```

## 4. 特殊标识符命名

### 4.1 模板参数命名
- 使用单个大写字母或描述性名称
- 遵循STL命名约定

```cpp
template <typename T>
class Container {
    // T代表类型
};

template <typename KeyType, typename ValueType>
class Map {
    // 描述性名称
};
```

### 4.2 异常类命名
- 以Exception结尾
- 描述异常类型

```cpp
class NetworkException;
class ValidationException;
class FileNotFoundException;
```

## 5. 命名约定检查

### 5.1 自动检查工具
推荐使用以下工具进行命名规范检查：
- clang-tidy
- cpplint
- 自定义脚本

### 5.2 代码审查要点
- [ ] 变量名是否具有描述性
- [ ] 函数名是否以动词开头
- [ ] 类名是否使用帕斯卡命名法
- [ ] 常量是否使用全大写命名
- [ ] 文件名是否符合规范

## 6. 示例代码

```cpp
// 完整的命名规范示例
namespace leee {

class UserAuthenticationManager {
public:
    UserAuthenticationManager();
    ~UserAuthenticationManager();

    bool authenticate_user(const std::string& username,
                          const std::string& password);

    void logout_user(int user_id);
    UserStatus get_user_status(int user_id);

private:
    void validate_credentials(const std::string& username,
                             const std::string& password);

    void update_last_login_time(int user_id);

    std::vector<User> users_;
    std::string database_connection_string_;
    int max_login_attempts_;
};

const int MAX_LOGIN_ATTEMPTS = 3;
const std::string DEFAULT_CONFIG_PATH = "/etc/leee/config.json";

} // namespace leee
```

---

*遵循这些命名规范将使代码更加一致和易于维护。*
