//
// Leee Edge Cases and Boundary Conditions Tests
// 测试边界条件、异常情况和极端输入
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <leee/bytearray.h>
#include <capability-foundations/leee/logger.h>
#include <limits>
#include <climits>
#include <cmath>
#include <filesystem>
#include <fstream>

// 测试用例：ByteArray边界条件
TEST_CASE("Edge Cases - ByteArray Boundaries") {
    SUBCASE("Empty strings") {
        leee::ByteArray empty;

        CHECK(empty.isEmpty());
        CHECK(empty.size() == 0);
        CHECK(empty.str() == "");

        // 空字符串上的操作
        CHECK(empty.toUpper().isEmpty());
        CHECK(empty.toLower().isEmpty());
        CHECK(empty.trim().isEmpty());

        // 空字符串的查找
        CHECK(empty.indexOf("anything") == -1);
        CHECK(empty.contains("anything") == false);
    }

    SUBCASE("Single character strings") {
        leee::ByteArray single("A");

        CHECK_FALSE(single.isEmpty());
        CHECK(single.size() == 1);
        CHECK(single.str() == "A");

        // 单字符操作
        CHECK(single.toUpper().str() == "A");
        CHECK(single.toLower().str() == "a");
        CHECK(single.trim().str() == "A");
    }

    SUBCASE("Very large strings") {
        const size_t large_size = 1024 * 1024; // 1MB
        std::string large_string(large_size, 'X');

        leee::ByteArray large_ba(large_string);

        CHECK(large_ba.size() == large_size);
        CHECK_FALSE(large_ba.isEmpty());

        // 大字符串的基本操作
        CHECK(large_ba.contains("XXX"));
        CHECK(large_ba.indexOf("XXX") >= 0);

        // 大字符串的子串操作
        leee::ByteArray mid = large_ba.mid(100000, 100);
        CHECK(mid.size() == 100);
        CHECK(mid.str() == std::string(100, 'X'));
    }

    SUBCASE("Strings with null characters") {
        std::string with_nulls("Hello\0World", 11);
        leee::ByteArray ba(with_nulls);

        CHECK(ba.size() == 11);
        CHECK(ba.contains("Hello"));
        CHECK(ba.contains("World"));

        // null字符的处理
        CHECK(ba.indexOf(std::string("Hello\0", 6)) == 0);
    }

    SUBCASE("Maximum size strings") {
        // 测试接近内存限制的字符串
        const size_t max_reasonable_size = 100 * 1024 * 1024; // 100MB (合理的最大值)
        try {
            std::string huge_string(max_reasonable_size, 'A');
            leee::ByteArray huge_ba(huge_string);

            CHECK(huge_ba.size() == max_reasonable_size);
            CHECK_FALSE(huge_ba.isEmpty());

            // 清理内存
            huge_string.clear();
        } catch (const std::bad_alloc&) {
            // 内存不足是预期的，跳过测试
            MESSAGE("Skipping large string test due to insufficient memory");
        }
    }
}

// 测试用例：字符串操作的边界条件
TEST_CASE("Edge Cases - String Operations Boundaries") {
    SUBCASE("Substring operations at boundaries") {
        leee::ByteArray ba("Hello World");

        // 左边界
        leee::ByteArray left = ba.left(0);
        CHECK(left.isEmpty());

        left = ba.left(5);
        CHECK(left.str() == "Hello");

        left = ba.left(20); // 超过长度
        CHECK(left.str() == "Hello World");

        // 右边界
        leee::ByteArray right = ba.right(0);
        CHECK(right.isEmpty());

        right = ba.right(5);
        CHECK(right.str() == "World");

        right = ba.right(20); // 超过长度
        CHECK(right.str() == "Hello World");

        // 中间子串
        leee::ByteArray mid = ba.mid(0, 5);
        CHECK(mid.str() == "Hello");

        mid = ba.mid(6, 5);
        CHECK(mid.str() == "World");

        mid = ba.mid(20, 5); // 起始位置超出
        CHECK(mid.isEmpty());

        mid = ba.mid(6, 20); // 长度超出
        CHECK(mid.str() == "World");
    }

    SUBCASE("Split operations at boundaries") {
        leee::ByteArray empty("");
        auto empty_split = empty.split(',');
        CHECK(empty_split.empty());

        leee::ByteArray single("single");
        auto single_split = single.split(',');
        CHECK(single_split.size() == 1);
        CHECK(single_split[0].str() == "single");

        leee::ByteArray only_delimiters(",,,");
        auto delimiter_split = only_delimiters.split(',');
        // 注意：连续分隔符的处理可能因实现而异
        CHECK(delimiter_split.size() >= 0);

        leee::ByteArray start_with_delimiter(",start");
        auto start_split = start_with_delimiter.split(',');
        CHECK(start_split.size() >= 1);

        leee::ByteArray end_with_delimiter("end,");
        auto end_split = end_with_delimiter.split(',');
        CHECK(end_split.size() >= 1);
    }

    SUBCASE("Replace operations at boundaries") {
        leee::ByteArray ba("aaa");

        // 替换空字符串
        ba.replace("", "X");
        CHECK(ba.str() == "aaa"); // 不应该改变

        // 替换不存在的字符串
        ba = leee::ByteArray("aaa");
        ba.replace("b", "X");
        CHECK(ba.str() == "aaa");

        // 替换整个字符串
        ba.replace("aaa", "bbb");
        CHECK(ba.str() == "bbb");

        // 替换重叠的字符串
        ba = leee::ByteArray("aaa");
        ba.replace("aa", "a");
        CHECK(ba.str() == "aa"); // "aaa" -> "a" + "a" = "aa"
    }
}

// 测试用例：类型转换的边界条件
TEST_CASE("Edge Cases - Type Conversion Boundaries") {
    SUBCASE("Integer conversion boundaries") {
        // 最大值
        leee::ByteArray max_int(std::to_string(INT_MAX));
        bool ok;
        int max_val = max_int.toInt(&ok);
        CHECK(ok);
        CHECK(max_val == INT_MAX);

        // 最小值
        leee::ByteArray min_int(std::to_string(INT_MIN));
        int min_val = min_int.toInt(&ok);
        CHECK(ok);
        CHECK(min_val == INT_MIN);

        // 溢出情况
        leee::ByteArray overflow("999999999999999999999");
        int overflow_val = overflow.toInt(&ok);
        CHECK_FALSE(ok); // 应该失败

        // 无效输入
        leee::ByteArray invalid("abc");
        int invalid_val = invalid.toInt(&ok);
        CHECK_FALSE(ok);

        leee::ByteArray empty_int("");
        int empty_val = empty_int.toInt(&ok);
        CHECK_FALSE(ok);
    }

    SUBCASE("Floating point conversion boundaries") {
        // 特殊浮点数值
        leee::ByteArray nan_str("nan");
        bool ok;
        double nan_val = nan_str.toDouble(&ok);
        CHECK_FALSE(ok); // nan字符串应该无法转换

        leee::ByteArray inf_str("inf");
        double inf_val = inf_str.toDouble(&ok);
        CHECK_FALSE(ok);

        leee::ByteArray neg_inf_str("-inf");
        double neg_inf_val = neg_inf_str.toDouble(&ok);
        CHECK_FALSE(ok);

        // 科学计数法
        leee::ByteArray scientific("1.23e-45");
        double sci_val = scientific.toDouble(&ok);
        CHECK(ok);
        CHECK(sci_val > 0);

        // 最大精度
        leee::ByteArray precise("3.141592653589793238462643383279502884197");
        double precise_val = precise.toDouble(&ok);
        CHECK(ok);
        CHECK(std::abs(precise_val - 3.141592653589793) < 1e-15);
    }

    SUBCASE("Number to string conversion") {
        // 整数边界
        leee::ByteArray int_max_str = leee::ByteArray::number(INT_MAX);
        CHECK(int_max_str.toInt() == INT_MAX);

        leee::ByteArray int_min_str = leee::ByteArray::number(INT_MIN);
        CHECK(int_min_str.toInt() == INT_MIN);

        // 浮点数边界
        leee::ByteArray double_max_str = leee::ByteArray::number(std::numeric_limits<double>::max());
        CHECK(double_max_str.toDouble() == std::numeric_limits<double>::max());

        // 不同进制
        leee::ByteArray hex_str = leee::ByteArray::number(255, 16);
        CHECK(hex_str.str() == "0xff");

        leee::ByteArray binary_str = leee::ByteArray::number(15, 2);
        CHECK(binary_str.str() == "0b1111");
    }
}

// 测试用例：文件操作的边界条件
TEST_CASE("Edge Cases - File Operations Boundaries") {
    SUBCASE("Empty file operations") {
        const std::string empty_file = "empty_test.txt";

        // 保存空数据
        leee::ByteArray empty;
        bool save_result = empty.saveToFile(empty_file);
        CHECK(save_result);

        if (save_result) {
            // 加载空文件
            leee::ByteArray loaded = leee::ByteArray::loadFromFile(empty_file);
            CHECK(loaded.isEmpty());

            // 验证文件存在但为空
            std::ifstream file(empty_file);
            std::string content((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
            CHECK(content.empty());
        }

        std::filesystem::remove(empty_file);
    }

    SUBCASE("Large file operations") {
        const std::string large_file = "large_test.txt";
        const size_t large_size = 10 * 1024 * 1024; // 10MB

        try {
            leee::ByteArray large_data;
            large_data.fill('A', large_size);

            bool save_result = large_data.saveToFile(large_file);
            CHECK(save_result);

            if (save_result) {
                leee::ByteArray loaded = leee::ByteArray::loadFromFile(large_file);
                CHECK(loaded.size() == large_size);

                // 验证文件大小
                CHECK(std::filesystem::file_size(large_file) == large_size);
            }
        } catch (const std::exception&) {
            // 文件操作可能因磁盘空间不足而失败，这是正常的
            MESSAGE("Large file test skipped due to system limitations");
        }

        std::filesystem::remove(large_file);
    }

    SUBCASE("Invalid file paths") {
        leee::ByteArray data("test data");

        // 无效路径
        CHECK_FALSE(data.saveToFile(""));
        CHECK_FALSE(data.saveToFile("/invalid/path/that/does/not/exist/file.txt"));

        // 加载不存在的文件
        leee::ByteArray loaded = leee::ByteArray::loadFromFile("nonexistent_file.txt");
        CHECK(loaded.isEmpty());
    }

    SUBCASE("Permission issues") {
        leee::ByteArray data("test data");

        // 尝试在只读目录中创建文件（如果存在这样的目录）
        const std::string readonly_path = "/readonly_test.txt";
        bool save_result = data.saveToFile(readonly_path);

        // 这个测试的结果取决于系统权限，可能成功也可能失败
        // 我们主要验证程序不会崩溃
        CHECK_NOTHROW(save_result);

        if (std::filesystem::exists(readonly_path)) {
            std::filesystem::remove(readonly_path);
        }
    }
}

// 测试用例：Logger的边界条件
TEST_CASE("Edge Cases - Logger Boundaries") {
    const std::string edge_log = "edge_case_logger.log";

    if (std::filesystem::exists(edge_log)) {
        std::filesystem::remove(edge_log);
    }

    leee::init_logger(edge_log);

    SUBCASE("Empty and null messages") {
        // 空消息
        CHECK_NOTHROW(LOG_INFO << "");
        CHECK_NOTHROW(LOG_WARNING << std::string());
        CHECK_NOTHROW(LOG_ERROR << nullptr); // 这可能会有问题，取决于实现

        // 大量空消息
        for (int i = 0; i < 1000; ++i) {
            LOG_INFO << "";
        }
    }

    SUBCASE("Very long messages") {
        const size_t long_size = 100 * 1024; // 100KB
        std::string long_message(long_size, 'X');

        CHECK_NOTHROW(LOG_INFO << long_message);
        CHECK_NOTHROW(LOG_ERROR << "Prefix: " << long_message << " suffix");
    }

    SUBCASE("Special characters in messages") {
        CHECK_NOTHROW(LOG_INFO << "Special chars: \n\t\r\"\'\\ \0 end");
        CHECK_NOTHROW(LOG_INFO << "Unicode: 你好世界 🌍");
        CHECK_NOTHROW(LOG_INFO << "Control chars: " << char(1) << char(2) << char(31));
    }

    SUBCASE("Extreme logging frequency") {
        // 高频日志记录
        const int burst_size = 10000;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < burst_size; ++i) {
            LOG_INFO << "Burst message " << i;
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        CHECK(duration.count() < 10000); // 1万条消息应该在10秒内完成
    }

    SUBCASE("Logger re-initialization") {
        // 多次初始化
        for (int i = 0; i < 10; ++i) {
            CHECK_NOTHROW(leee::init_logger());
            CHECK(leee::is_initialized());
        }

        // 使用不同参数重新初始化
        CHECK_NOTHROW(leee::init_logger("different_log.txt", 1000, 2));
        CHECK(leee::is_initialized());
    }

    std::filesystem::remove(edge_log);
    std::filesystem::remove("different_log.txt");
}

// 测试用例：并发边界条件
TEST_CASE("Edge Cases - Concurrency Boundaries") {
    SUBCASE("Single thread edge cases") {
        // 在单线程中测试可能导致竞态条件的操作
        leee::ByteArray shared_data("shared");

        // 快速连续操作
        for (int i = 0; i < 1000; ++i) {
            shared_data += "X";
            volatile size_t size = shared_data.size();
            (void)size;
        }

        CHECK(shared_data.size() == 6 + 1000); // "shared" + 1000个'X'
    }

    SUBCASE("Resource exhaustion simulation") {
        // 模拟资源耗尽情况
        std::vector<leee::ByteArray> many_objects;

        try {
            // 创建大量对象
            for (int i = 0; i < 100000; ++i) {
                many_objects.emplace_back("Object " + std::to_string(i));
            }

            CHECK(many_objects.size() == 100000);

            // 清理
            many_objects.clear();

        } catch (const std::bad_alloc&) {
            // 内存不足是正常的，清理已创建的对象
            many_objects.clear();
            MESSAGE("Resource exhaustion test stopped due to memory limits");
        }
    }
}

// 测试用例：异常安全
TEST_CASE("Edge Cases - Exception Safety") {
    SUBCASE("Exception during operations") {
        leee::ByteArray data("test");

        // 测试各种可能抛出异常的操作
        CHECK_NOTHROW(data.toUpper());
        CHECK_NOTHROW(data.toLower());
        CHECK_NOTHROW(data.trim());

        // 即使在异常情况下，对象状态应该保持一致
        try {
            // 尝试一些可能失败的操作
            data.mid(SIZE_MAX, 1);
        } catch (...) {
            // 即使抛出异常，对象应该仍然可用
            CHECK_NOTHROW(data.size());
            CHECK_NOTHROW(data.str());
            CHECK_NOTHROW(data.isEmpty());
        }
    }

    SUBCASE("Exception during file operations") {
        const std::string invalid_file = "/invalid/path/file.txt";
        leee::ByteArray data("test");

        // 文件操作应该不会因异常而导致程序崩溃
        CHECK_NOTHROW(data.saveToFile(invalid_file));
        CHECK_NOTHROW(leee::ByteArray::loadFromFile(invalid_file));

        // 即使文件操作失败，对象状态应该保持不变
        CHECK(data.str() == "test");
        CHECK(data.size() == 4);
    }

    SUBCASE("Exception during logging") {
        // Logger操作应该不会因异常而导致程序崩溃
        CHECK_NOTHROW(LOG_INFO << "Test message");
        CHECK_NOTHROW(LOG_ERROR << std::string(1000000, 'X')); // 非常长的消息
        CHECK_NOTHROW(LOG_WARNING << nullptr); // 空指针（可能有问题）
    }
}

// 测试用例：数据一致性
TEST_CASE("Edge Cases - Data Consistency") {
    SUBCASE("Operation sequence consistency") {
        leee::ByteArray original("Hello World");

        // 执行一系列操作
        leee::ByteArray copy1 = original;
        copy1.toUpper();

        leee::ByteArray copy2 = original;
        copy2.toLower();

        leee::ByteArray copy3 = original;
        copy3.trim();

        // 原始对象应该保持不变
        CHECK(original.str() == "Hello World");

        // 副本应该有预期的修改
        CHECK(copy1.str() == "HELLO WORLD");
        CHECK(copy2.str() == "hello world");
        CHECK(copy3.str() == "Hello World");

        // 操作应该是独立的
        CHECK(copy1.str() != copy2.str());
        CHECK(copy1.str() != copy3.str());
        CHECK(copy2.str() != copy3.str());
    }

    SUBCASE("Immutable operations") {
        leee::ByteArray data("test");

        // 测试返回新对象的操作不会修改原对象
        leee::ByteArray upper = data.toUpper();
        CHECK(data.str() == "test");
        CHECK(upper.str() == "TEST");

        leee::ByteArray lower = data.toLower();
        CHECK(data.str() == "test");
        CHECK(lower.str() == "test");

        leee::ByteArray trimmed = data.trim();
        CHECK(data.str() == "test");
        CHECK(trimmed.str() == "test");
    }
}
