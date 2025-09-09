//
/ Leee Logger Unit Tests
// 使用 doctest 框架进行单元测试
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <logger.h>
#include <filesystem>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

// 测试用例：Logger初始化
TEST_CASE("Logger - Initialization") {
    SUBCASE("Default initialization") {
        // 重置logger状态（如果已初始化）
        plog::init(plog::none, nullptr);

        leee::init_logger();
        CHECK(leee::is_initialized());
        CHECK(leee::get_log_level() == plog::verbose);
    }

    SUBCASE("Custom log file initialization") {
        const std::string test_log_file = "test_custom.log";

        // 清理之前的测试文件
        if (std::filesystem::exists(test_log_file)) {
            std::filesystem::remove(test_log_file);
        }

        leee::init_logger(test_log_file, 50000, 3);
        CHECK(leee::is_initialized());

        // 验证日志文件是否创建
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        CHECK(std::filesystem::exists(test_log_file));

        // 清理测试文件
        std::filesystem::remove(test_log_file);
    }

    SUBCASE("Multiple initialization calls") {
        leee::init_logger();
        bool first_init = leee::is_initialized();

        leee::init_logger();
        bool second_init = leee::is_initialized();

        CHECK(first_init);
        CHECK(second_init);
    }
}

// 测试用例：日志级别设置
TEST_CASE("Logger - Log Level Management") {
    leee::init_logger();

    SUBCASE("Set and get log level") {
        leee::set_log_level(plog::info);
        CHECK(leee::get_log_level() == plog::info);

        leee::set_log_level(plog::error);
        CHECK(leee::get_log_level() == plog::error);

        leee::set_log_level(plog::debug);
        CHECK(leee::get_log_level() == plog::debug);
    }

    SUBCASE("Invalid log level handling") {
        // 测试边界情况
        leee::set_log_level(static_cast<plog::Severity>(999));
        CHECK(leee::get_log_level() >= plog::none);
        CHECK(leee::get_log_level() <= plog::verbose);
    }
}

// 测试用例：日志输出测试
TEST_CASE("Logger - Log Output") {
    const std::string test_log_file = "test_output.log";

    // 清理之前的测试文件
    if (std::filesystem::exists(test_log_file)) {
        std::filesystem::remove(test_log_file);
    }

    leee::init_logger(test_log_file);

    SUBCASE("Basic log messages") {
        LOG_INFO << "Test info message";
        LOG_WARNING << "Test warning message";
        LOG_ERROR << "Test error message";

        // 等待日志写入
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // 验证日志文件内容
        CHECK(std::filesystem::exists(test_log_file));
        std::ifstream log_file(test_log_file);
        std::string content((std::istreambuf_iterator<char>(log_file)),
                           std::istreambuf_iterator<char>());

        CHECK(content.find("Test info message") != std::string::npos);
        CHECK(content.find("Test warning message") != std::string::npos);
        CHECK(content.find("Test error message") != std::string::npos);
    }

    SUBCASE("Stream operators") {
        int test_value = 42;
        double test_double = 3.14159;
        std::string test_string = "test";

        LOG_INFO << "Integer: " << test_value << ", Double: " << test_double << ", String: " << test_string;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::ifstream log_file(test_log_file);
        std::string content((std::istreambuf_iterator<char>(log_file)),
                           std::istreambuf_iterator<char>());

        CHECK(content.find("Integer: 42") != std::string::npos);
        CHECK(content.find("Double: 3.14159") != std::string::npos);
        CHECK(content.find("String: test") != std::string::npos);
    }

    SUBCASE("Log level filtering") {
        leee::set_log_level(plog::error);

        LOG_INFO << "This should not appear";
        LOG_WARNING << "This should not appear";
        LOG_ERROR << "This should appear";

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::ifstream log_file(test_log_file);
        std::string content((std::istreambuf_iterator<char>(log_file)),
                           std::istreambuf_iterator<char>());

        CHECK(content.find("This should not appear") == std::string::npos);
        CHECK(content.find("This should appear") != std::string::npos);
    }

    // 清理测试文件
    std::filesystem::remove(test_log_file);
}

// 测试用例：多线程日志测试
TEST_CASE("Logger - Multi-threading") {
    const std::string test_log_file = "test_thread.log";

    if (std::filesystem::exists(test_log_file)) {
        std::filesystem::remove(test_log_file);
    }

    leee::init_logger(test_log_file);

    SUBCASE("Concurrent logging") {
        const int num_threads = 5;
        const int logs_per_thread = 10;
        std::vector<std::thread> threads;

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([i, logs_per_thread]() {
                for (int j = 0; j < logs_per_thread; ++j) {
                    LOG_INFO << "Thread " << i << " - Message " << j;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        // 等待所有日志写入
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // 验证日志文件
        std::ifstream log_file(test_log_file);
        std::string content((std::istreambuf_iterator<char>(log_file)),
                           std::istreambuf_iterator<char>());

        // 应该有 num_threads * logs_per_thread 条日志
        int log_count = 0;
        size_t pos = 0;
        while ((pos = content.find("Thread", pos)) != std::string::npos) {
            ++log_count;
            pos += 6;
        }

        CHECK(log_count == num_threads * logs_per_thread);
    }

    std::filesystem::remove(test_log_file);
}

// 测试用例：日志格式化
TEST_CASE("Logger - Formatting") {
    const std::string test_log_file = "test_format.log";

    if (std::filesystem::exists(test_log_file)) {
        std::filesystem::remove(test_log_file);
    }

    leee::init_logger(test_log_file);

    SUBCASE("GLOG-style formatting") {
        LOG_INFO << "Test message";

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::ifstream log_file(test_log_file);
        std::string line;
        bool found_valid_format = false;

        while (std::getline(log_file, line)) {
            // 检查GLOG格式：I MMDD HH:MM:SS.micros thread file:line] message
            if (line.length() > 20 &&
                (line[0] == 'I' || line[0] == 'W' || line[0] == 'E' || line[0] == 'F') &&
                line.find(']') != std::string::npos &&
                line.find("Test message") != std::string::npos) {
                found_valid_format = true;
                break;
            }
        }

        CHECK(found_valid_format);
    }

    std::filesystem::remove(test_log_file);
}

// 测试用例：日志轮转
TEST_CASE("Logger - Log Rotation") {
    const std::string base_log_file = "test_rotation.log";
    const size_t small_file_size = 100; // 很小的文件大小来测试轮转

    // 清理之前的测试文件
    for (int i = 0; i < 5; ++i) {
        std::string filename = base_log_file + (i == 0 ? "" : "." + std::to_string(i));
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }

    leee::init_logger(base_log_file, small_file_size, 3);

    SUBCASE("File rotation on size limit") {
        // 写入足够多的日志来触发文件轮转
        std::string long_message(50, 'X'); // 50个字符的消息

        for (int i = 0; i < 10; ++i) {
            LOG_INFO << "Message " << i << ": " << long_message;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // 检查是否有轮转文件生成
        bool has_rotation_files = false;
        for (int i = 1; i <= 3; ++i) {
            std::string rotated_file = base_log_file + "." + std::to_string(i);
            if (std::filesystem::exists(rotated_file)) {
                has_rotation_files = true;
                break;
            }
        }

        CHECK(has_rotation_files);
    }

    // 清理测试文件
    for (int i = 0; i < 5; ++i) {
        std::string filename = base_log_file + (i == 0 ? "" : "." + std::to_string(i));
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }
}

// 测试用例：错误处理
TEST_CASE("Logger - Error Handling") {
    SUBCASE("Logging without initialization") {
        // 重置logger
        plog::init(plog::none, nullptr);

        // 尝试在未初始化的情况下记录日志（应该不会崩溃）
        CHECK_NOTHROW(LOG_INFO << "This should not crash");

        // 验证logger未初始化
        CHECK_FALSE(leee::is_initialized());
    }

    SUBCASE("Invalid file path") {
        const std::string invalid_path = "/invalid/path/that/does/not/exist/log.txt";

        // 应该不会因为无效路径而崩溃
        CHECK_NOTHROW(leee::init_logger(invalid_path));

        // 即使路径无效，logger也应该被认为是初始化的
        CHECK(leee::is_initialized());
    }
}

// 测试用例：性能测试
TEST_CASE("Logger - Performance") {
    const std::string perf_log_file = "test_performance.log";

    if (std::filesystem::exists(perf_log_file)) {
        std::filesystem::remove(perf_log_file);
    }

    leee::init_logger(perf_log_file);

    SUBCASE("High frequency logging") {
        const int num_messages = 1000;
        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < num_messages; ++i) {
            LOG_INFO << "Performance test message " << i;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        // 性能应该在合理范围内（每条消息不应超过1ms）
        CHECK(duration.count() < num_messages);
    }

    std::filesystem::remove(perf_log_file);
}

// 测试用例：LogStream类
TEST_CASE("Logger - LogStream Class") {
    SUBCASE("Stream operations") {
        leee::init_logger();

        // 测试流操作（不会实际输出，因为这是单元测试）
        leee::LogStream stream(plog::info, __FILE__, __LINE__, __FUNCTION__);

        CHECK_NOTHROW(stream << "Test value: " << 42 << std::endl);
    }

    SUBCASE("Stream manipulators") {
        leee::LogStream stream(plog::info);

        CHECK_NOTHROW(stream << std::hex << 255 << std::dec << " " << 42);
    }
}
