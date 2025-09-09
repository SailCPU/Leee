# 错误处理规范

## 概述

良好的错误处理是构建可靠软件的基础。本文档定义了Leee项目的错误处理策略、异常使用规范和错误恢复机制。

## 1. 错误处理原则

### 1.1 异常安全保证
- **基本保证 (Basic Guarantee)**: 操作失败时，程序状态保持有效
- **强保证 (Strong Guarantee)**: 操作要么完全成功，要么完全回滚
- **不抛出保证 (No-throw Guarantee)**: 操作保证不会抛出异常

### 1.2 错误处理层次
```
┌─────────────────────────────────────┐
│         用户界面层                   │
│         (User Interface)            │
├─────────────────────────────────────┤
│         业务逻辑层                   │
│         (Business Logic)            │
├─────────────────────────────────────┤
│         数据访问层                   │
│         (Data Access)               │
├─────────────────────────────────────┤
│         系统层                       │
│         (System Layer)              │
└─────────────────────────────────────┘
```

## 2. 异常层次结构

### 2.1 基础异常类
定义统一的异常基类：

```cpp
namespace leee {

class Exception : public std::exception {
public:
    explicit Exception(const std::string& message)
        : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

    virtual const char* type() const noexcept {
        return "Exception";
    }

private:
    std::string message_;
};

// 运行时异常
class RuntimeException : public Exception {
public:
    using Exception::Exception;

    const char* type() const noexcept override {
        return "RuntimeException";
    }
};

// 逻辑异常
class LogicException : public Exception {
public:
    using Exception::Exception;

    const char* type() const noexcept override {
        return "LogicException";
    }
};

} // namespace leee
```

### 2.2 专用异常类
针对不同模块定义专用异常：

```cpp
namespace leee {

// 网络异常
class NetworkException : public RuntimeException {
public:
    NetworkException(const std::string& message,
                    const std::string& host = "",
                    int port = 0)
        : RuntimeException(message)
        , host_(host)
        , port_(port) {}

    const char* type() const noexcept override {
        return "NetworkException";
    }

    const std::string& host() const { return host_; }
    int port() const { return port_; }

private:
    std::string host_;
    int port_;
};

// 文件系统异常
class FileException : public RuntimeException {
public:
    FileException(const std::string& message,
                 const std::string& file_path = "")
        : RuntimeException(message)
        , file_path_(file_path) {}

    const char* type() const noexcept override {
        return "FileException";
    }

    const std::string& file_path() const { return file_path_; }

private:
    std::string file_path_;
};

// 配置异常
class ConfigurationException : public LogicException {
public:
    using LogicException::LogicException;

    const char* type() const noexcept override {
        return "ConfigurationException";
    }
};

} // namespace leee
```

## 3. 错误处理策略

### 3.1 构造函数错误处理
构造函数应避免抛出异常，使用两阶段构造：

```cpp
class DatabaseConnection {
public:
    // 不抛出异常的构造函数
    DatabaseConnection(const std::string& connection_string)
        : connection_string_(connection_string)
        , connected_(false) {}

    // 显式连接方法
    bool connect() {
        try {
            // 连接逻辑
            connected_ = true;
            return true;
        } catch (const std::exception& e) {
            connected_ = false;
            return false;
        }
    }

private:
    std::string connection_string_;
    bool connected_;
};

// 推荐用法
DatabaseConnection conn("postgresql://localhost/db");
if (!conn.connect()) {
    throw ConnectionException("Failed to connect to database");
}
```

### 3.2 RAII资源管理
使用RAII确保资源安全释放：

```cpp
class FileHandler {
public:
    explicit FileHandler(const std::string& path)
        : file_(path) {
        if (!file_.is_open()) {
            throw FileException("Cannot open file: " + path, path);
        }
    }

    ~FileHandler() {
        if (file_.is_open()) {
            file_.close();
        }
    }

    // 禁用拷贝
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    // 支持移动
    FileHandler(FileHandler&& other) noexcept
        : file_(std::move(other.file_)) {}

    void write(const std::string& data) {
        if (!file_.is_open()) {
            throw FileException("File is not open");
        }
        file_ << data;
    }

private:
    std::ofstream file_;
};
```

### 3.3 函数异常规范
明确函数的异常行为：

```cpp
/**
 * @brief 从文件中读取数据
 * @param file_path 文件路径
 * @return 读取的数据
 * @throw FileException 文件不存在或无法读取
 * @throw std::bad_alloc 内存分配失败
 */
std::string read_file(const std::string& file_path) {
    FileHandler file(file_path);

    std::stringstream buffer;
    std::string line;

    while (std::getline(file.file_, line)) {
        buffer << line << '\n';
    }

    return buffer.str();
}

/**
 * @brief 安全的文件读取，不会抛出异常
 * @param file_path 文件路径
 * @param content 输出参数，读取的内容
 * @return 是否读取成功
 */
bool read_file_safe(const std::string& file_path,
                   std::string& content) noexcept {
    try {
        content = read_file(file_path);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}
```

## 4. 错误恢复策略

### 4.1 重试机制
实现智能重试逻辑：

```cpp
template <typename Func, typename ExceptionType = std::exception>
auto retry(Func func, int max_attempts = 3,
           std::chrono::milliseconds delay = std::chrono::milliseconds(100))
    -> decltype(func()) {

    ExceptionType last_exception("");

    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        try {
            return func();
        } catch (const ExceptionType& e) {
            last_exception = e;

            if (attempt < max_attempts - 1) {
                std::this_thread::sleep_for(delay);
                delay *= 2; // 指数退避
            }
        }
    }

    throw last_exception;
}

// 使用示例
auto result = retry([]() {
    return connect_to_server("example.com", 80);
}, 5, std::chrono::milliseconds(500));
```

### 4.2 降级处理
实现优雅降级：

```cpp
class ServiceManager {
public:
    void process_request(const Request& request) {
        try {
            // 尝试使用完整功能
            return process_with_full_features(request);
        } catch (const NetworkException& e) {
            // 网络异常，降级到本地处理
            return process_with_local_cache(request);
        } catch (const DatabaseException& e) {
            // 数据库异常，降级到默认值
            return process_with_defaults(request);
        } catch (const std::exception& e) {
            // 未知异常，记录错误并返回错误响应
            logger_.error("Unexpected error: " + std::string(e.what()));
            return create_error_response(request);
        }
    }

private:
    Response process_with_full_features(const Request& request);
    Response process_with_local_cache(const Request& request);
    Response process_with_defaults(const Request& request);
    Response create_error_response(const Request& request);

    Logger logger_;
};
```

### 4.3 断路器模式
防止级联故障：

```cpp
class CircuitBreaker {
public:
    enum class State { CLOSED, OPEN, HALF_OPEN };

    template <typename Func>
    auto execute(Func func) -> decltype(func()) {
        std::lock_guard<std::mutex> lock(mutex_);

        if (state_ == State::OPEN) {
            if (should_attempt_reset()) {
                state_ = State::HALF_OPEN;
            } else {
                throw ServiceUnavailableException("Circuit breaker is open");
            }
        }

        try {
            auto result = func();
            on_success();
            return result;
        } catch (const std::exception& e) {
            on_failure();
            throw;
        }
    }

private:
    void on_success() {
        failure_count_ = 0;
        state_ = State::CLOSED;
    }

    void on_failure() {
        ++failure_count_;
        if (failure_count_ >= failure_threshold_) {
            state_ = State::OPEN;
            last_failure_time_ = std::chrono::steady_clock::now();
        }
    }

    bool should_attempt_reset() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - last_failure_time_).count();
        return elapsed >= reset_timeout_seconds_;
    }

    State state_ = State::CLOSED;
    int failure_count_ = 0;
    int failure_threshold_ = 5;
    int reset_timeout_seconds_ = 60;
    std::chrono::steady_clock::time_point last_failure_time_;
    std::mutex mutex_;
};
```

## 5. 日志和监控

### 5.1 错误日志记录
规范化错误日志：

```cpp
class ErrorLogger {
public:
    void log_error(const std::exception& e,
                  const std::string& context = "",
                  const std::source_location& location = std::source_location::current()) {

        std::stringstream ss;
        ss << "[ERROR] " << location.file_name() << ":"
           << location.line() << " in " << location.function_name() << "\n"
           << "Context: " << context << "\n"
           << "Exception: " << e.what() << "\n"
           << "Type: " << typeid(e).name();

        // 写入日志文件
        write_to_log(ss.str());

        // 在调试模式下输出到控制台
#ifdef DEBUG
        std::cerr << ss.str() << std::endl;
#endif
    }

private:
    void write_to_log(const std::string& message);
};
```

### 5.2 错误指标收集
收集错误统计信息：

```cpp
class ErrorMetrics {
public:
    void record_error(const std::exception& e,
                     const std::string& operation) {
        std::lock_guard<std::mutex> lock(mutex_);

        error_counts_[operation]++;
        error_types_[typeid(e).name()]++;

        // 记录错误发生时间
        error_timestamps_[operation].push_back(
            std::chrono::system_clock::now()
        );

        // 清理旧的记录
        cleanup_old_records();
    }

    void report_metrics() const {
        std::cout << "Error Metrics Report:\n";
        for (const auto& [operation, count] : error_counts_) {
            std::cout << "  " << operation << ": " << count << " errors\n";
        }
    }

private:
    void cleanup_old_records() {
        auto cutoff = std::chrono::system_clock::now() -
                     std::chrono::hours(24);

        for (auto& [operation, timestamps] : error_timestamps_) {
            auto it = std::remove_if(
                timestamps.begin(), timestamps.end(),
                [cutoff](const auto& ts) { return ts < cutoff; }
            );
            timestamps.erase(it, timestamps.end());
        }
    }

    std::map<std::string, int> error_counts_;
    std::map<std::string, int> error_types_;
    std::map<std::string, std::vector<std::chrono::system_clock::time_point>>
        error_timestamps_;
    mutable std::mutex mutex_;
};
```

## 6. 最佳实践

### 6.1 异常使用准则
- 只在异常情况下使用异常
- 不要使用异常进行普通控制流
- 异常消息应该清晰描述问题
- 避免在析构函数中抛出异常

### 6.2 错误处理模式
```cpp
// 模式1: 直接处理
try {
    risky_operation();
} catch (const SpecificException& e) {
    handle_specific_error(e);
} catch (const std::exception& e) {
    handle_generic_error(e);
}

// 模式2: 转换和重新抛出
try {
    low_level_operation();
} catch (const LowLevelException& e) {
    throw HighLevelException("High level operation failed", e);
}

// 模式3: 资源管理
{
    ResourceGuard guard(resource);
    perform_operation(resource);
} // guard自动清理资源
```

### 6.3 测试错误处理
```cpp
TEST_CASE("Error Handling - File Operations") {
    SUBCASE("Invalid File Path") {
        CHECK_THROWS_AS(read_file("/invalid/path"), FileException);
    }

    SUBCASE("Permission Denied") {
        CHECK_THROWS_AS(read_file("/root/system.conf"),
                       FileException);
    }

    SUBCASE("Safe Read Function") {
        std::string content;
        CHECK_FALSE(read_file_safe("/invalid/path", content));
        CHECK(content.empty());
    }
}
```

---

*良好的错误处理让软件更加健壮和可靠。*
