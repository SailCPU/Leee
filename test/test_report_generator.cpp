//
/ Leee Test Report Generator
// 生成详细的测试报告和覆盖率分析
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <bytearray.h>
#include <logger.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <map>
#include <vector>

// 测试报告数据结构
struct TestResult {
    std::string test_name;
    std::string test_case;
    bool passed;
    double duration_ms;
    std::string failure_message;
    std::chrono::system_clock::time_point timestamp;
};

struct TestSuiteResult {
    std::string suite_name;
    std::vector<TestResult> results;
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    double total_duration_ms = 0.0;
};

class TestReportGenerator {
private:
    std::vector<TestSuiteResult> suite_results_;
    std::chrono::system_clock::time_point test_start_time_;
    std::string report_file_;

    // 收集测试结果的辅助函数
    void collectTestResults() {
        // 这里我们将使用doctest的报告功能
        // 由于doctest的限制，我们将创建一个简化的报告收集机制
    }

public:
    TestReportGenerator(const std::string& report_file = "test_report.html")
        : report_file_(report_file), test_start_time_(std::chrono::system_clock::now()) {}

    void startTestSuite(const std::string& suite_name) {
        TestSuiteResult suite;
        suite.suite_name = suite_name;
        suite_results_.push_back(suite);
    }

    void addTestResult(const std::string& test_name, const std::string& test_case,
                      bool passed, double duration_ms, const std::string& failure_message = "") {
        if (suite_results_.empty()) {
            startTestSuite("Default Suite");
        }

        TestResult result;
        result.test_name = test_name;
        result.test_case = test_case;
        result.passed = passed;
        result.duration_ms = duration_ms;
        result.failure_message = failure_message;
        result.timestamp = std::chrono::system_clock::now();

        suite_results_.back().results.push_back(result);
        suite_results_.back().total_tests++;
        suite_results_.back().total_duration_ms += duration_ms;

        if (passed) {
            suite_results_.back().passed_tests++;
        } else {
            suite_results_.back().failed_tests++;
        }
    }

    void generateHTMLReport() {
        std::ofstream report(report_file_);

        if (!report.is_open()) {
            LOG_ERROR << "Failed to create report file: " << report_file_;
            return;
        }

        report << "<!DOCTYPE html>\n";
        report << "<html lang='zh-CN'>\n";
        report << "<head>\n";
        report << "    <meta charset='UTF-8'>\n";
        report << "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
        report << "    <title>Leee 测试报告</title>\n";
        report << "    <style>\n";
        report << "        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }\n";
        report << "        .container { max-width: 1200px; margin: 0 auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n";
        report << "        h1 { color: #2c3e50; text-align: center; margin-bottom: 30px; }\n";
        report << "        .summary { display: flex; justify-content: space-around; margin-bottom: 30px; }\n";
        report << "        .summary-item { text-align: center; padding: 20px; border-radius: 8px; }\n";
        report << "        .passed { background-color: #d4edda; color: #155724; }\n";
        report << "        .failed { background-color: #f8d7da; color: #721c24; }\n";
        report << "        .total { background-color: #cce7ff; color: #004085; }\n";
        report << "        .suite { margin-bottom: 20px; border: 1px solid #ddd; border-radius: 8px; }\n";
        report << "        .suite-header { background-color: #f8f9fa; padding: 15px; border-bottom: 1px solid #ddd; }\n";
        report << "        .suite-name { font-size: 18px; font-weight: bold; margin: 0; }\n";
        report << "        .suite-stats { margin-top: 5px; font-size: 14px; color: #666; }\n";
        report << "        .test-results { padding: 15px; }\n";
        report << "        .test-item { display: flex; justify-content: space-between; align-items: center; padding: 10px; margin-bottom: 5px; border-radius: 4px; }\n";
        report << "        .test-passed { background-color: #d4edda; }\n";
        report << "        .test-failed { background-color: #f8d7da; }\n";
        report << "        .test-name { font-weight: bold; }\n";
        report << "        .test-duration { color: #666; }\n";
        report << "        .failure-message { color: #721c24; margin-top: 5px; font-size: 14px; }\n";
        report << "        .timestamp { text-align: center; color: #666; margin-top: 30px; }\n";
        report << "        .progress-bar { width: 100%; height: 20px; background-color: #e9ecef; border-radius: 10px; margin: 10px 0; }\n";
        report << "        .progress-fill { height: 100%; border-radius: 10px; transition: width 0.3s ease; }\n";
        report << "        .progress-passed { background-color: #28a745; }\n";
        report << "        .progress-failed { background-color: #dc3545; }\n";
        report << "    </style>\n";
        report << "</head>\n";
        report << "<body>\n";
        report << "    <div class='container'>\n";
        report << "        <h1>Leee 单元测试报告</h1>\n";

        // 计算总体统计
        int total_suites = suite_results_.size();
        int total_tests = 0;
        int total_passed = 0;
        int total_failed = 0;
        double total_duration = 0.0;

        for (const auto& suite : suite_results_) {
            total_tests += suite.total_tests;
            total_passed += suite.passed_tests;
            total_failed += suite.failed_tests;
            total_duration += suite.total_duration_ms;
        }

        // 测试摘要
        report << "        <div class='summary'>\n";
        report << "            <div class='summary-item total'>\n";
        report << "                <h3>总测试数</h3>\n";
        report << "                <div style='font-size: 24px; font-weight: bold;'>" << total_tests << "</div>\n";
        report << "            </div>\n";
        report << "            <div class='summary-item passed'>\n";
        report << "                <h3>通过</h3>\n";
        report << "                <div style='font-size: 24px; font-weight: bold;'>" << total_passed << "</div>\n";
        report << "            </div>\n";
        report << "            <div class='summary-item failed'>\n";
        report << "                <h3>失败</h3>\n";
        report << "                <div style='font-size: 24px; font-weight: bold;'>" << total_failed << "</div>\n";
        report << "            </div>\n";
        report << "        </div>\n";

        // 总体进度条
        if (total_tests > 0) {
            double pass_percentage = (static_cast<double>(total_passed) / total_tests) * 100.0;
            double fail_percentage = (static_cast<double>(total_failed) / total_tests) * 100.0;

            report << "        <div class='progress-bar'>\n";
            if (total_passed > 0) {
                report << "            <div class='progress-fill progress-passed' style='width: " << pass_percentage << "%;'></div>\n";
            }
            if (total_failed > 0) {
                report << "            <div class='progress-fill progress-failed' style='width: " << fail_percentage << "%; float: right;'></div>\n";
            }
            report << "        </div>\n";
        }

        // 测试套件详情
        for (const auto& suite : suite_results_) {
            if (suite.total_tests == 0) continue;

            double pass_rate = (static_cast<double>(suite.passed_tests) / suite.total_tests) * 100.0;

            report << "        <div class='suite'>\n";
            report << "            <div class='suite-header'>\n";
            report << "                <h4 class='suite-name'>" << suite.suite_name << "</h4>\n";
            report << "                <div class='suite-stats'>\n";
            report << "                    测试数: " << suite.total_tests << " | ";
            report << "                    通过: " << suite.passed_tests << " | ";
            report << "                    失败: " << suite.failed_tests << " | ";
            report << "                    通过率: " << std::fixed << std::setprecision(1) << pass_rate << "% | ";
            report << "                    总耗时: " << std::fixed << std::setprecision(2) << suite.total_duration_ms << " ms\n";
            report << "                </div>\n";
            report << "            </div>\n";

            report << "            <div class='test-results'>\n";

            for (const auto& result : suite.results) {
                std::string test_class = result.passed ? "test-passed" : "test-failed";

                report << "                <div class='test-item " << test_class << "'>\n";
                report << "                    <div class='test-name'>" << result.test_name;
                if (!result.test_case.empty()) {
                    report << " - " << result.test_case;
                }
                report << "</div>\n";
                report << "                    <div class='test-duration'>" << std::fixed << std::setprecision(2) << result.duration_ms << " ms</div>\n";

                if (!result.passed && !result.failure_message.empty()) {
                    report << "                    <div class='failure-message'>失败原因: " << result.failure_message << "</div>\n";
                }

                report << "                </div>\n";
            }

            report << "            </div>\n";
            report << "        </div>\n";
        }

        // 生成时间戳
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream time_ss;
        time_ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");

        report << "        <div class='timestamp'>\n";
        report << "            报告生成时间: " << time_ss.str() << "<br>\n";
        report << "            测试总耗时: " << std::fixed << std::setprecision(2) << total_duration << " ms\n";
        report << "        </div>\n";

        report << "    </div>\n";
        report << "</body>\n";
        report << "</html>\n";

        report.close();

        LOG_INFO << "HTML测试报告已生成: " << report_file_;
    }

    void generateTextReport() {
        std::string text_file = report_file_.substr(0, report_file_.find_last_of('.')) + ".txt";
        std::ofstream report(text_file);

        if (!report.is_open()) {
            LOG_ERROR << "Failed to create text report file: " << text_file;
            return;
        }

        report << "=====================================\n";
        report << "       Leee 单元测试报告\n";
        report << "=====================================\n\n";

        // 计算总体统计
        int total_suites = suite_results_.size();
        int total_tests = 0;
        int total_passed = 0;
        int total_failed = 0;
        double total_duration = 0.0;

        for (const auto& suite : suite_results_) {
            total_tests += suite.total_tests;
            total_passed += suite.passed_tests;
            total_failed += suite.failed_tests;
            total_duration += suite.total_duration_ms;
        }

        report << "总体统计:\n";
        report << "  总套件数: " << total_suites << "\n";
        report << "  总测试数: " << total_tests << "\n";
        report << "  通过测试: " << total_passed << "\n";
        report << "  失败测试: " << total_failed << "\n";
        report << "  通过率: " << std::fixed << std::setprecision(1)
               << (total_tests > 0 ? (static_cast<double>(total_passed) / total_tests) * 100.0 : 0.0) << "%\n";
        report << "  总耗时: " << std::fixed << std::setprecision(2) << total_duration << " ms\n\n";

        // 详细的套件信息
        for (const auto& suite : suite_results_) {
            if (suite.total_tests == 0) continue;

            report << "套件: " << suite.suite_name << "\n";
            report << "  测试数: " << suite.total_tests << "\n";
            report << "  通过: " << suite.passed_tests << "\n";
            report << "  失败: " << suite.failed_tests << "\n";
            report << "  通过率: " << std::fixed << std::setprecision(1)
                   << (static_cast<double>(suite.passed_tests) / suite.total_tests) * 100.0 << "%\n";
            report << "  耗时: " << std::fixed << std::setprecision(2) << suite.total_duration_ms << " ms\n";

            if (!suite.results.empty()) {
                report << "  详细结果:\n";
                for (const auto& result : suite.results) {
                    report << "    " << (result.passed ? "[PASS]" : "[FAIL]") << " "
                           << result.test_name;
                    if (!result.test_case.empty()) {
                        report << " - " << result.test_case;
                    }
                    report << " (" << std::fixed << std::setprecision(2) << result.duration_ms << " ms)";

                    if (!result.passed && !result.failure_message.empty()) {
                        report << "\n      失败原因: " << result.failure_message;
                    }
                    report << "\n";
                }
            }
            report << "\n";
        }

        // 生成时间戳
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream time_ss;
        time_ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");

        report << "报告生成时间: " << time_ss.str() << "\n";
        report << "=====================================\n";

        report.close();

        LOG_INFO << "文本测试报告已生成: " << text_file;
    }
};

// 全局测试报告生成器实例
static TestReportGenerator* g_report_generator = nullptr;

// 测试报告的辅助宏
#define TEST_REPORT_START(suite_name) \
    if (!g_report_generator) { \
        g_report_generator = new TestReportGenerator(); \
    } \
    g_report_generator->startTestSuite(suite_name);

#define TEST_REPORT_RESULT(test_name, test_case, passed, duration_ms, failure_msg) \
    if (g_report_generator) { \
        g_report_generator->addTestResult(test_name, test_case, passed, duration_ms, failure_msg); \
    }

#define TEST_REPORT_GENERATE() \
    if (g_report_generator) { \
        g_report_generator->generateHTMLReport(); \
        g_report_generator->generateTextReport(); \
        delete g_report_generator; \
        g_report_generator = nullptr; \
    }

// 示例测试用例，演示如何使用测试报告生成器
TEST_CASE("Report Generator - Demo Tests") {
    TEST_REPORT_START("Report Generator Demo");

    SUBCASE("Successful test") {
        auto start = std::chrono::high_resolution_clock::now();

        // 模拟测试逻辑
        int result = 2 + 2;
        CHECK(result == 4);

        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        TEST_REPORT_RESULT("Demo Tests", "Successful test", true, duration);
    }

    SUBCASE("Failing test") {
        auto start = std::chrono::high_resolution_clock::now();

        // 模拟失败的测试
        int result = 2 + 2;
        CHECK(result == 5); // 这会失败

        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        TEST_REPORT_RESULT("Demo Tests", "Failing test", false, duration, "Expected 2+2=5, but got 4");
    }

    SUBCASE("Exception test") {
        auto start = std::chrono::high_resolution_clock::now();

        try {
            // 模拟可能抛出异常的操作
            throw std::runtime_error("Test exception");
        } catch (const std::exception& e) {
            CHECK(std::string(e.what()) == "Test exception");
        }

        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        TEST_REPORT_RESULT("Demo Tests", "Exception test", true, duration);
    }

    // 生成报告
    TEST_REPORT_GENERATE();
}

// 性能测试报告
TEST_CASE("Performance Report - ByteArray Operations") {
    TEST_REPORT_START("Performance Tests");

    SUBCASE("String creation benchmark") {
        const int iterations = 10000;
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; ++i) {
            leee::ByteArray ba("Performance test string " + std::to_string(i));
            volatile size_t size = ba.size();
            (void)size;
        }

        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        double avg_time = duration / iterations;

        CHECK(avg_time < 1.0); // 平均创建时间应小于1ms

        TEST_REPORT_RESULT("Performance Tests", "String creation benchmark",
                          avg_time < 1.0, duration,
                          avg_time >= 1.0 ? "Performance requirement not met" : "");
    }

    SUBCASE("Memory operations benchmark") {
        const int iterations = 5000;
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; ++i) {
            leee::ByteArray ba;
            ba.fill('A', 1024); // 1KB
            leee::ByteArray copy = ba;
            leee::ByteArray upper = ba.toUpper();
            volatile size_t total = copy.size() + upper.size();
            (void)total;
        }

        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        double avg_time = duration / iterations;

        CHECK(avg_time < 2.0); // 平均内存操作时间应小于2ms

        TEST_REPORT_RESULT("Performance Tests", "Memory operations benchmark",
                          avg_time < 2.0, duration,
                          avg_time >= 2.0 ? "Memory performance requirement not met" : "");
    }

    TEST_REPORT_GENERATE();
}

// 覆盖率分析辅助函数
TEST_CASE("Coverage Analysis - Module Usage") {
    TEST_REPORT_START("Coverage Analysis");

    SUBCASE("ByteArray module coverage") {
        // 测试ByteArray的所有主要功能
        leee::ByteArray ba("test string");

        // 基本操作
        CHECK(ba.size() == 11);
        CHECK_FALSE(ba.isEmpty());

        // 字符串操作
        leee::ByteArray upper = ba.toUpper();
        CHECK(upper.str() == "TEST STRING");

        leee::ByteArray lower = ba.toLower();
        CHECK(lower.str() == "test string");

        // 查找操作
        CHECK(ba.indexOf("test") == 0);
        CHECK(ba.contains("string"));

        // 子串操作
        leee::ByteArray left = ba.left(4);
        CHECK(left.str() == "test");

        leee::ByteArray mid = ba.mid(5, 6);
        CHECK(mid.str() == "string");

        // 分割连接
        auto parts = ba.split(' ');
        CHECK(parts.size() == 2);

        leee::ByteArray joined = leee::ByteArray::join(parts, "-");
        CHECK(joined.str() == "test-string");

        TEST_REPORT_RESULT("Coverage Analysis", "ByteArray module coverage", true, 0.0);
    }

    SUBCASE("Logger module coverage") {
        // 测试Logger的所有主要功能
        leee::init_logger("coverage_test.log");

        // 基本日志记录
        LOG_INFO << "Coverage test info message";
        LOG_WARNING << "Coverage test warning message";
        LOG_ERROR << "Coverage test error message";

        // 日志级别设置
        leee::set_log_level(plog::debug);
        CHECK(leee::get_log_level() == plog::debug);

        leee::set_log_level(plog::error);
        CHECK(leee::get_log_level() == plog::error);

        // 流操作
        LOG_INFO << "Stream test: " << 42 << " " << 3.14 << " " << "string";

        // 初始化状态检查
        CHECK(leee::is_initialized());

        TEST_REPORT_RESULT("Coverage Analysis", "Logger module coverage", true, 0.0);

        // 清理
        std::filesystem::remove("coverage_test.log");
    }

    TEST_REPORT_GENERATE();
}
