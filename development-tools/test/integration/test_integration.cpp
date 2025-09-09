//
// Leee Integration Tests
// 测试多个模块之间的交互和集成功能
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <bytearray.h>
#include <logger.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

// 测试用例：ByteArray和Logger的集成
TEST_CASE("Integration - ByteArray and Logger") {
    const std::string test_log_file = "integration_test.log";

    // 清理之前的测试文件
    if (std::filesystem::exists(test_log_file)) {
        std::filesystem::remove(test_log_file);
    }

    leee::init_logger(test_log_file);

    SUBCASE("Logging ByteArray operations") {
        leee::ByteArray test_data("Integration test data");

        // 执行各种操作并记录日志
        LOG_INFO << "Original data: " << test_data.str();
        LOG_INFO << "Data size: " << test_data.size();

        test_data.toUpper();
        LOG_INFO << "After toUpper(): " << test_data.str();

        test_data.append(" APPEND");
        LOG_INFO << "After append(): " << test_data.str();

        leee::ByteArray substring = test_data.mid(5, 10);
        LOG_INFO << "Substring (5,10): " << substring.str();

        // 验证日志文件包含了所有操作记录
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::ifstream log_file(test_log_file);
        std::string log_content((std::istreambuf_iterator<char>(log_file)),
                               std::istreambuf_iterator<char>());

        CHECK(log_content.find("Original data") != std::string::npos);
        CHECK(log_content.find("Data size") != std::string::npos);
        CHECK(log_content.find("After toUpper()") != std::string::npos);
        CHECK(log_content.find("After append()") != std::string::npos);
        CHECK(log_content.find("Substring") != std::string::npos);
    }

    SUBCASE("Error handling with logging") {
        leee::ByteArray data("test data");

        try {
            // 尝试访问无效索引
            volatile char invalid_char = data[data.size()]; // 越界访问
            (void)invalid_char;

            LOG_ERROR << "Unexpected: No exception thrown for out-of-bounds access";
        } catch (const std::exception& e) {
            LOG_ERROR << "Expected exception caught: " << e.what();
        } catch (...) {
            LOG_ERROR << "Unknown exception caught during ByteArray operation";
        }

        // 验证错误被正确记录
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::ifstream log_file(test_log_file);
        std::string log_content((std::istreambuf_iterator<char>(log_file)),
                               std::istreambuf_iterator<char>());

        CHECK((log_content.find("exception caught") != std::string::npos ||
               log_content.find("Error") != std::string::npos));
    }

    // 清理
    std::filesystem::remove(test_log_file);
}

// 测试用例：文件操作集成
TEST_CASE("Integration - File Operations") {
    const std::string test_file = "integration_file_test.txt";
    const std::string log_file = "integration_file_log.log";

    // 清理之前的测试文件
    if (std::filesystem::exists(test_file)) {
        std::filesystem::remove(test_file);
    }
    if (std::filesystem::exists(log_file)) {
        std::filesystem::remove(log_file);
    }

    leee::init_logger(log_file);

    SUBCASE("ByteArray file operations with logging") {
        // 创建测试数据
        leee::ByteArray original_data("This is test data for file integration testing.\n");
        original_data += "It contains multiple lines and various content.\n";
        original_data += "Timestamp: " + leee::ByteArray::getCurrentTimeString().str();

        LOG_INFO << "Saving data to file: " << test_file;
        bool save_result = original_data.saveToFile(test_file);
        CHECK(save_result);

        if (save_result) {
            LOG_INFO << "File saved successfully, size: " << std::filesystem::file_size(test_file) << " bytes";

            // 从文件加载数据
            LOG_INFO << "Loading data from file";
            leee::ByteArray loaded_data = leee::ByteArray::loadFromFile(test_file);

            LOG_INFO << "Loaded data size: " << loaded_data.size() << " bytes";
            CHECK(loaded_data.size() == original_data.size());
            CHECK(loaded_data.str() == original_data.str());

            // 验证文件内容
            std::ifstream file_stream(test_file);
            std::string file_content((std::istreambuf_iterator<char>(file_stream)),
                                   std::istreambuf_iterator<char>());

            CHECK(file_content == original_data.str());
            LOG_INFO << "File content verification passed";
        } else {
            LOG_ERROR << "Failed to save file: " << test_file;
        }
    }

    SUBCASE("Multiple file operations") {
        std::vector<std::string> test_files = {"test1.txt", "test2.txt", "test3.txt"};
        std::vector<leee::ByteArray> test_data;

        // 创建多个测试文件
        for (size_t i = 0; i < test_files.size(); ++i) {
            leee::ByteArray data("File " + std::to_string(i + 1) + " content: " +
                               std::string(i * 10, 'A' + i) + "\n");

            test_data.push_back(data);
            LOG_INFO << "Creating file " << test_files[i];

            bool save_result = data.saveToFile(test_files[i]);
            CHECK(save_result);

            if (save_result) {
                LOG_INFO << "File " << test_files[i] << " created successfully";
            }
        }

        // 验证所有文件
        for (size_t i = 0; i < test_files.size(); ++i) {
            leee::ByteArray loaded = leee::ByteArray::loadFromFile(test_files[i]);
            CHECK(loaded.str() == test_data[i].str());
            LOG_INFO << "File " << test_files[i] << " verification passed";
        }

        // 清理文件
        for (const auto& file : test_files) {
            std::filesystem::remove(file);
        }
    }

    // 清理
    std::filesystem::remove(test_file);
    std::filesystem::remove(log_file);
}

// 测试用例：数据处理流水线
TEST_CASE("Integration - Data Processing Pipeline") {
    const std::string pipeline_log = "pipeline_test.log";

    if (std::filesystem::exists(pipeline_log)) {
        std::filesystem::remove(pipeline_log);
    }

    leee::init_logger(pipeline_log);

    SUBCASE("Complete data processing workflow") {
        // 1. 数据输入
        std::string raw_input = "  raw DATA with Mixed CASE and   extra   spaces  ";
        leee::ByteArray input_data(raw_input);

        LOG_INFO << "Step 1: Input data received: '" << input_data.str() << "'";

        // 2. 数据清理
        input_data.trim();
        LOG_INFO << "Step 2: After trim: '" << input_data.str() << "'";

        // 3. 数据转换
        input_data.toLower();
        LOG_INFO << "Step 3: After toLower: '" << input_data.str() << "'";

        // 4. 数据分割
        auto words = input_data.split(' ');
        LOG_INFO << "Step 4: Split into " << words.size() << " words";

        // 5. 数据过滤（移除空字符串）
        std::vector<leee::ByteArray> filtered_words;
        for (const auto& word : words) {
            if (!word.isEmpty()) {
                filtered_words.push_back(word);
            }
        }
        LOG_INFO << "Step 5: After filtering empty words: " << filtered_words.size() << " words";

        // 6. 数据重组
        leee::ByteArray result = leee::ByteArray::join(filtered_words, "_");
        LOG_INFO << "Step 6: Final result: '" << result.str() << "'";

        // 7. 数据输出
        const std::string output_file = "pipeline_output.txt";
        result.saveToFile(output_file);
        LOG_INFO << "Step 7: Result saved to file: " << output_file;

        // 验证结果
        CHECK(result.str() == "raw_data_with_mixed_case_and_extra_spaces");

        // 清理
        std::filesystem::remove(output_file);
    }

    SUBCASE("Performance monitoring pipeline") {
        leee::PerformanceUtil perf;
        perf.reset();

        LOG_INFO << "Starting performance-monitored pipeline";

        // 执行一些操作
        perf.recordTimePoint();
        leee::ByteArray data("Performance test data");
        for (int i = 0; i < 1000; ++i) {
            data += " additional content";
        }
        perf.recordTimePoint();

        // 更多操作
        leee::ByteArray processed = data.toUpper();
        perf.recordTimePoint();

        // 文件操作
        processed.saveToFile("perf_pipeline_test.txt");
        perf.recordTimePoint();

        LOG_INFO << "Pipeline completed";
        LOG_INFO << "Performance report: " << perf.getReport().str();

        // 验证性能数据
        auto intervals = perf.getIntervals();
        CHECK(intervals.size() == 3); // 4个时间点产生3个间隔

        for (size_t i = 0; i < intervals.size(); ++i) {
            LOG_INFO << "Interval " << i << ": " << intervals[i] << " ms";
            CHECK(intervals[i] >= 0.0);
        }

        // 清理
        std::filesystem::remove("perf_pipeline_test.txt");
    }

    std::filesystem::remove(pipeline_log);
}

// 测试用例：多线程集成
TEST_CASE("Integration - Multi-threading") {
    const std::string mt_log_file = "multithread_test.log";
    const int num_threads = 4;
    const int operations_per_thread = 100;

    if (std::filesystem::exists(mt_log_file)) {
        std::filesystem::remove(mt_log_file);
    }

    leee::init_logger(mt_log_file);

    SUBCASE("Concurrent ByteArray and Logger operations") {
        std::vector<std::thread> threads;
        std::atomic<int> completed_operations(0);

        LOG_INFO << "Starting multi-threaded integration test with " << num_threads << " threads";

        for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
            threads.emplace_back([thread_id, operations_per_thread, &completed_operations]() {
                for (int i = 0; i < operations_per_thread; ++i) {
                    // 创建ByteArray并执行操作
                    leee::ByteArray data("Thread " + std::to_string(thread_id) + " - Operation " + std::to_string(i));
                    data.toUpper();

                    // 记录日志
                    LOG_INFO << "Thread " << thread_id << " completed operation " << i
                            << " (data size: " << data.size() << ")";

                    completed_operations++;
                }
            });
        }

        // 等待所有线程完成
        for (auto& thread : threads) {
            thread.join();
        }

        LOG_INFO << "All threads completed. Total operations: " << completed_operations.load();

        // 验证所有操作都已完成
        CHECK(completed_operations.load() == num_threads * operations_per_thread);

        // 验证日志文件包含了所有线程的操作记录
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        std::ifstream log_file(mt_log_file);
        std::string log_content((std::istreambuf_iterator<char>(log_file)),
                               std::istreambuf_iterator<char>());

        // 检查每个线程的日志记录
        for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
            std::string thread_marker = "Thread " + std::to_string(thread_id) + " completed operation";
            CHECK(log_content.find(thread_marker) != std::string::npos);
        }
    }

    SUBCASE("Shared resource access") {
        std::mutex log_mutex;
        const std::string shared_file = "shared_resource_test.txt";

        if (std::filesystem::exists(shared_file)) {
            std::filesystem::remove(shared_file);
        }

        auto shared_operation = [&](int thread_id, int iterations) {
            for (int i = 0; i < iterations; ++i) {
                {
                    std::lock_guard<std::mutex> lock(log_mutex);
                    LOG_INFO << "Thread " << thread_id << " - Iteration " << i;

                    // 共享文件操作
                    leee::ByteArray data("Shared data from thread " + std::to_string(thread_id) +
                                       " iteration " + std::to_string(i) + "\n");

                    // 追加到共享文件
                    data.appendToFile(shared_file);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        };

        std::vector<std::thread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(shared_operation, i, 50);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        // 验证共享文件内容
        leee::ByteArray shared_content = leee::ByteArray::loadFromFile(shared_file);
        CHECK_FALSE(shared_content.isEmpty());

        // 检查文件包含了所有线程的数据
        std::string content_str = shared_content.str();
        for (int i = 0; i < 3; ++i) {
            std::string thread_marker = "Shared data from thread " + std::to_string(i);
            CHECK(content_str.find(thread_marker) != std::string::npos);
        }

        LOG_INFO << "Shared resource test completed successfully";

        // 清理
        std::filesystem::remove(shared_file);
    }

    std::filesystem::remove(mt_log_file);
}

// 测试用例：配置和初始化集成
TEST_CASE("Integration - Configuration and Initialization") {
    SUBCASE("Logger and ByteArray initialization sequence") {
        // 重置状态
        plog::init(plog::none, nullptr);

        // 1. 初始化logger
        leee::init_logger();
        CHECK(leee::is_initialized());

        // 2. 使用ByteArray进行配置数据处理
        leee::ByteArray config_data("log_level=info;max_file_size=1048576;max_files=5");
        auto config_pairs = config_data.split(';');

        std::map<std::string, std::string> config_map;
        for (const auto& pair : config_pairs) {
            auto key_value = pair.split('=');
            if (key_value.size() == 2) {
                config_map[key_value[0].str()] = key_value[1].str();
            }
        }

        // 3. 根据配置调整logger设置
        if (config_map.count("log_level")) {
            if (config_map["log_level"] == "debug") {
                leee::set_log_level(plog::debug);
            } else if (config_map["log_level"] == "info") {
                leee::set_log_level(plog::info);
            }
        }

        // 4. 记录配置信息
        LOG_INFO << "Configuration loaded successfully";
        LOG_INFO << "Log level set to: " << config_map["log_level"];
        LOG_INFO << "Max file size: " << config_map["max_file_size"];
        LOG_INFO << "Max files: " << config_map["max_files"];

        // 验证配置生效
        CHECK(leee::get_log_level() == plog::info);
    }
}

// 测试用例：错误恢复和容错
TEST_CASE("Integration - Error Recovery") {
    const std::string error_log = "error_recovery_test.log";

    if (std::filesystem::exists(error_log)) {
        std::filesystem::remove(error_log);
    }

    leee::init_logger(error_log);

    SUBCASE("Graceful degradation on file system errors") {
        const std::string invalid_path = "/invalid/path/that/does/not/exist/test.txt";
        const std::string valid_path = "recovery_test.txt";

        leee::ByteArray data("Test data for error recovery");

        // 尝试保存到无效路径（应该失败但不崩溃）
        LOG_INFO << "Attempting to save to invalid path: " << invalid_path;
        bool invalid_save_result = data.saveToFile(invalid_path);
        if (!invalid_save_result) {
            LOG_WARNING << "Save to invalid path failed as expected";
        }

        // 保存到有效路径
        LOG_INFO << "Saving to valid path: " << valid_path;
        bool valid_save_result = data.saveToFile(valid_path);
        CHECK(valid_save_result);

        if (valid_save_result) {
            LOG_INFO << "Successfully recovered and saved to valid path";

            // 验证数据完整性
            leee::ByteArray loaded = leee::ByteArray::loadFromFile(valid_path);
            CHECK(loaded.str() == data.str());
            LOG_INFO << "Data integrity verified after error recovery";
        }

        // 清理
        std::filesystem::remove(valid_path);
    }

    SUBCASE("Logger fallback behavior") {
        // 测试logger在各种错误情况下的行为
        LOG_INFO << "Testing logger fallback behavior";

        // 记录各种类型的数据
        LOG_INFO << "String: " << std::string("test");
        LOG_INFO << "Number: " << 42;
        LOG_INFO << "Float: " << 3.14159;

        // 测试大消息
        std::string large_message(5000, 'X');
        LOG_INFO << "Large message size: " << large_message.size();

        // 测试特殊字符
        LOG_INFO << "Special chars: \n\t\r\"\'\\";

        LOG_INFO << "Logger fallback test completed successfully";
    }

    std::filesystem::remove(error_log);
}
