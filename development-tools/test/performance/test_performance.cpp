//
// Leee Performance Tests
// 性能测试套件，用于评估各个模块的性能表现
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <leee/bytearray.h>
#include <capability-foundations/leee/logger.h>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <filesystem>

// 性能测试辅助类
class PerformanceTimer {
private:
    std::string test_name_;
    std::chrono::high_resolution_clock::time_point start_time_;

public:
    explicit PerformanceTimer(const std::string& test_name)
        : test_name_(test_name), start_time_(std::chrono::high_resolution_clock::now()) {}

    ~PerformanceTimer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_);
        std::cout << "[PERF] " << test_name_ << ": " << duration.count() << " microseconds" << std::endl;
    }

    double getElapsedMs() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_);
        return duration.count();
    }
};

// ByteArray性能测试
TEST_CASE("Performance - ByteArray Operations") {
    SUBCASE("String creation performance") {
        const int iterations = 10000;

        PerformanceTimer timer("ByteArray String Creation");
        for (int i = 0; i < iterations; ++i) {
            leee::ByteArray ba("Performance test string with some content");
            // 简单的操作来避免编译器优化
            volatile size_t size = ba.size();
            (void)size;
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Average ByteArray creation time: " << avg_time << " ms" << std::endl;
        CHECK(avg_time < 1.0); // 平均创建时间应小于1ms
    }

    SUBCASE("String concatenation performance") {
        const int iterations = 5000;
        const std::string base_str = "Test string ";

        PerformanceTimer timer("ByteArray Concatenation");
        for (int i = 0; i < iterations; ++i) {
            leee::ByteArray result;
            for (int j = 0; j < 10; ++j) {
                result += base_str + std::to_string(j);
            }
            volatile size_t size = result.size();
            (void)size;
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Average concatenation time: " << avg_time << " ms" << std::endl;
        CHECK(avg_time < 5.0); // 平均连接时间应小于5ms
    }

    SUBCASE("String search performance") {
        const int iterations = 10000;
        leee::ByteArray large_text;
        for (int i = 0; i < 100; ++i) {
            large_text += "This is a test string for performance measurement. ";
        }

        PerformanceTimer timer("ByteArray Search");
        for (int i = 0; i < iterations; ++i) {
            volatile int pos = large_text.indexOf("performance");
            (void)pos;
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Average search time: " << avg_time << " ms" << std::endl;
        CHECK(avg_time < 0.1); // 平均搜索时间应小于0.1ms
    }

    SUBCASE("File operations performance") {
        const int iterations = 100;
        const std::string test_file = "perf_test_file.txt";
        const std::string content = "This is a test content for file performance measurement.\n";

        // 创建较大的测试内容
        std::string large_content;
        for (int i = 0; i < 1000; ++i) {
            large_content += content;
        }
        leee::ByteArray test_data(large_content);

        PerformanceTimer timer("ByteArray File Operations");
        for (int i = 0; i < iterations; ++i) {
            // 写文件
            test_data.saveToFile(test_file);

            // 读文件
            leee::ByteArray loaded = leee::ByteArray::loadFromFile(test_file);

            volatile size_t size = loaded.size();
            (void)size;
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Average file operation time: " << avg_time << " ms" << std::endl;
        CHECK(avg_time < 50.0); // 平均文件操作时间应小于50ms

        // 清理
        std::filesystem::remove(test_file);
    }

    SUBCASE("Memory operations performance") {
        const int iterations = 10000;
        const size_t data_size = 1024; // 1KB

        PerformanceTimer timer("ByteArray Memory Operations");

        for (int i = 0; i < iterations; ++i) {
            // 创建数据
            leee::ByteArray data;
            data.fill('A', data_size);

            // 执行各种操作
            leee::ByteArray copy = data;
            leee::ByteArray upper = data.toUpper();
            leee::ByteArray substring = data.mid(100, 100);

            volatile size_t total_size = copy.size() + upper.size() + substring.size();
            (void)total_size;
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Average memory operation time: " << avg_time << " ms" << std::endl;
        CHECK(avg_time < 1.0); // 平均内存操作时间应小于1ms
    }
}

// Logger性能测试
TEST_CASE("Performance - Logger Operations") {
    const std::string perf_log_file = "perf_logger_test.log";

    // 清理之前的文件
    if (std::filesystem::exists(perf_log_file)) {
        std::filesystem::remove(perf_log_file);
    }

    leee::init_logger(perf_log_file);

    SUBCASE("Basic logging performance") {
        const int iterations = 10000;

        PerformanceTimer timer("Logger Basic Operations");
        for (int i = 0; i < iterations; ++i) {
            LOG_INFO << "Performance test message " << i << " with some additional data: " << (i * 3.14159);
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Average logging time: " << avg_time << " ms per message" << std::endl;
        CHECK(avg_time < 0.5); // 平均日志记录时间应小于0.5ms
    }

    SUBCASE("Concurrent logging performance") {
        const int num_threads = 4;
        const int logs_per_thread = 1000;

        PerformanceTimer timer("Logger Concurrent Operations");

        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([i, logs_per_thread]() {
                for (int j = 0; j < logs_per_thread; ++j) {
                    LOG_INFO << "Thread " << i << " - Message " << j;
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        double elapsed = timer.getElapsedMs();
        double total_messages = num_threads * logs_per_thread;
        double avg_time = elapsed / total_messages;

        std::cout << "[PERF] Average concurrent logging time: " << avg_time << " ms per message" << std::endl;
        CHECK(avg_time < 1.0); // 平均并发日志记录时间应小于1.0ms
    }

    SUBCASE("Different log levels performance") {
        const int iterations = 5000;

        // 测试不同日志级别的性能
        std::vector<std::pair<std::string, std::function<void(int)>>> log_operations = {
            {"INFO", [](int i) { LOG_INFO << "Info message " << i; }},
            {"WARNING", [](int i) { LOG_WARNING << "Warning message " << i; }},
            {"ERROR", [](int i) { LOG_ERROR << "Error message " << i; }}
        };

        for (auto& [level_name, log_func] : log_operations) {
            PerformanceTimer level_timer("Logger " + level_name + " Level");

            for (int i = 0; i < iterations; ++i) {
                log_func(i);
            }

            double elapsed = level_timer.getElapsedMs();
            double avg_time = elapsed / iterations;
            std::cout << "[PERF] " << level_name << " logging: " << avg_time << " ms per message" << std::endl;
        }
    }

    SUBCASE("Large message logging performance") {
        const int iterations = 100;
        std::string large_message(10000, 'X'); // 10KB消息

        PerformanceTimer timer("Logger Large Messages");
        for (int i = 0; i < iterations; ++i) {
            LOG_INFO << "Large message " << i << ": " << large_message;
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Large message logging: " << avg_time << " ms per message" << std::endl;
        CHECK(avg_time < 10.0); // 大消息日志记录时间应小于10ms
    }

    // 清理
    std::filesystem::remove(perf_log_file);
}

// 综合性能测试
TEST_CASE("Performance - Integrated Operations") {
    const std::string integrated_log_file = "perf_integrated_test.log";

    if (std::filesystem::exists(integrated_log_file)) {
        std::filesystem::remove(integrated_log_file);
    }

    leee::init_logger(integrated_log_file);

    SUBCASE("ByteArray and Logger integration") {
        const int iterations = 1000;

        PerformanceTimer timer("Integrated ByteArray-Logger Operations");

        for (int i = 0; i < iterations; ++i) {
            // 创建ByteArray并进行操作
            leee::ByteArray data("Test data for iteration ");
            data += std::to_string(i);
            data.toUpper();

            // 记录操作结果
            LOG_INFO << "Processed data: " << data.str() << " (size: " << data.size() << ")";

            // 文件操作
            std::string temp_file = "temp_" + std::to_string(i) + ".txt";
            data.saveToFile(temp_file);

            // 清理临时文件
            std::filesystem::remove(temp_file);
        }

        double elapsed = timer.getElapsedMs();
        double avg_time = elapsed / iterations;

        std::cout << "[PERF] Integrated operations: " << avg_time << " ms per iteration" << std::endl;
        CHECK(avg_time < 5.0); // 集成操作时间应小于5ms
    }

    SUBCASE("Memory usage estimation") {
        const int iterations = 10000;
        std::vector<leee::ByteArray> containers;

        PerformanceTimer timer("Memory Usage Test");

        // 创建大量ByteArray对象来测试内存使用
        for (int i = 0; i < iterations; ++i) {
            leee::ByteArray ba("Memory test data " + std::to_string(i));
            containers.push_back(ba);
        }

        double elapsed = timer.getElapsedMs();

        // 记录内存使用统计
        size_t total_size = 0;
        for (const auto& ba : containers) {
            total_size += ba.size();
        }

        std::cout << "[PERF] Memory test completed in " << elapsed << " ms" << std::endl;
        std::cout << "[PERF] Total data size: " << total_size << " bytes" << std::endl;
        std::cout << "[PERF] Average object size: " << (total_size / iterations) << " bytes" << std::endl;

        CHECK(elapsed < 1000.0); // 内存测试应在1秒内完成
    }

    // 清理
    std::filesystem::remove(integrated_log_file);
}

// 压力测试
TEST_CASE("Performance - Stress Tests") {
    SUBCASE("High frequency operations") {
        const int iterations = 100000;

        PerformanceTimer timer("Stress Test - High Frequency");

        for (int i = 0; i < iterations; ++i) {
            leee::ByteArray ba(std::to_string(i));
            volatile char first_char = ba[0];
            (void)first_char;
        }

        double elapsed = timer.getElapsedMs();
        double ops_per_second = (iterations * 1000.0) / elapsed;

        std::cout << "[PERF] Stress test: " << ops_per_second << " operations/second" << std::endl;
        CHECK(ops_per_second > 10000); // 应至少支持10,000次操作/秒
    }

    SUBCASE("Large data processing") {
        const size_t large_size = 1024 * 1024; // 1MB
        leee::ByteArray large_data;
        large_data.fill('A', large_size);

        PerformanceTimer timer("Stress Test - Large Data");

        // 执行各种操作
        leee::ByteArray copy = large_data;
        leee::ByteArray upper = large_data.toUpper();
        leee::ByteArray reversed = large_data;

        // 反转字符串（模拟复杂操作）
        for (size_t i = 0; i < large_size / 2; ++i) {
            char temp = reversed[i];
            reversed[i] = reversed[large_size - 1 - i];
            reversed[large_size - 1 - i] = temp;
        }

        double elapsed = timer.getElapsedMs();

        std::cout << "[PERF] Large data processing: " << elapsed << " ms for 1MB data" << std::endl;
        CHECK(elapsed < 1000.0); // 1MB数据处理应在1秒内完成
    }
}

// 基准测试与其他实现的比较
TEST_CASE("Performance - Benchmarks") {
    SUBCASE("String operations comparison") {
        const int iterations = 10000;

        // 测试std::string
        {
            PerformanceTimer timer("std::string operations");
            for (int i = 0; i < iterations; ++i) {
                std::string str = "test string " + std::to_string(i);
                std::string upper_str = str;
                for (char& c : upper_str) {
                    c = std::toupper(c);
                }
                volatile size_t len = str.length();
                (void)len;
            }
        }

        // 测试ByteArray
        {
            PerformanceTimer timer("ByteArray operations");
            for (int i = 0; i < iterations; ++i) {
                leee::ByteArray ba("test string " + std::to_string(i));
                leee::ByteArray upper_ba = ba.toUpper();
                volatile size_t len = ba.size();
                (void)len;
            }
        }

        std::cout << "[PERF] Benchmark comparison completed" << std::endl;
    }
}
