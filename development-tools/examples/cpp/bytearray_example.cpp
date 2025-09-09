//
// Leee ByteArray Module Example
// 演示如何使用 Leee 项目的字节数组工具模块
//

#include <leee/bytearray.h>
#include <capability-foundations/leee/logger.h>
#include <iostream>
#include <vector>

int main()
{
    // 初始化日志
    leee::init_logger();

    std::cout << "=== Leee ByteArray Example ===\n";

    // 1. 基本字符串操作
    leee::ByteArray str("Hello, World! This is a test string.");

    LOG_INFO << "Original string: " << str.str();
    LOG_INFO << "Left 5 chars: " << str.left(5).str();
    LOG_INFO << "Right 5 chars: " << str.right(5).str();
    LOG_INFO << "Starts with 'Hello': " << str.startsWith("Hello");

    // 2. 字符串分割
    auto parts = str.split(' ');
    LOG_INFO << "Split by space, parts count: " << parts.size();
    for (size_t i = 0; i < parts.size() && i < 3; ++i) {
        LOG_INFO << "Part " << i << ": " << parts[i].str();
    }

    // 3. 字符串查找
    size_t pos = str.indexOf('W');
    LOG_INFO << "Position of 'W': " << pos;

    // 4. 大小写转换
    leee::ByteArray caseStr("Hello World");
    LOG_INFO << "Original: " << caseStr.str();
    LOG_INFO << "Lower: " << caseStr.toLower().str();
    LOG_INFO << "Upper: " << caseStr.toUpper().str();

    // 5. 数字转换
    leee::ByteArray numStr("42");
    int intValue = numStr.toInt();
    LOG_INFO << "String to int: " << intValue;

    // 6. 浮点数转换
    leee::ByteArray floatStr("3.14159");
    double pi = floatStr.toDouble();
    LOG_INFO << "String to double: " << pi;

    // 7. 字符串包含检查
    bool hasWorld = str.contains("World");
    LOG_INFO << "Contains 'World': " << hasWorld;

    // 8. 文件操作
    leee::ByteArray fileContent("This is test file content.\nWith multiple lines.");
    fileContent.saveToFile("test_output.txt");
    LOG_INFO << "File saved to test_output.txt";

    leee::ByteArray loadedContent = leee::ByteArray::loadFromFile("test_output.txt");
    LOG_INFO << "Loaded from file: " << loadedContent.str();

    // 9. 时间戳
    double timestamp = leee::ByteArray::getCurrentTimeStampTicksMs();
    LOG_INFO << "Current timestamp (ms): " << timestamp;

    // 10. 性能分析示例
    leee::PerformanceUtil perf;
    perf.reset();

    // 模拟一些操作
    for (int i = 0; i < 10000; ++i) {
        leee::ByteArray temp = leee::ByteArray("test").toUpper();
    }
    perf.recordTimePoint();

    for (int i = 0; i < 5000; ++i) {
        auto parts = leee::ByteArray("another test").split(' ');
    }
    perf.recordTimePoint();

    leee::ByteArray perfResult = perf.getReport();
    LOG_INFO << "Performance analysis: " << perfResult.str();

    // 11. 字符串追加
    leee::ByteArray combined = leee::ByteArray("Hello") + leee::ByteArray(", ") + leee::ByteArray("World!");
    LOG_INFO << "Combined string: " << combined.str();

    // 12. 路径操作
    leee::ByteArray filePath("/home/user/documents/test.txt");
    LOG_INFO << "Full path: " << filePath.str();
    LOG_INFO << "File name: " << leee::ByteArray::getFileName(filePath.str()).str();
    LOG_INFO << "Directory: " << leee::ByteArray::getDirectory(filePath.str()).str();
    LOG_INFO << "Extension: " << leee::ByteArray::getFileExtension(filePath.str()).str();

    LOG_INFO << "ByteArray example completed successfully!";

    return 0;
}