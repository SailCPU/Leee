//
// Leee ByteArray Unit Tests
// 使用 doctest 框架进行单元测试
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <leee/bytearray.h>
#include <capability-foundations/leee/logger.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// 初始化日志以便在测试中输出信息
static bool logger_initialized = []() {
    leee::init_logger();
    return true;
}();

// 测试用例：基本构造和初始化
TEST_CASE("ByteArray - Basic Construction") {
    SUBCASE("Default constructor") {
        leee::ByteArray ba;
        CHECK(ba.isEmpty());
        CHECK(ba.size() == 0);
    }

    SUBCASE("String constructor") {
        leee::ByteArray ba("Hello World");
        CHECK_FALSE(ba.isEmpty());
        CHECK(ba.size() == 11);
        CHECK(ba.str() == "Hello World");
    }

    SUBCASE("Copy constructor") {
        leee::ByteArray original("Test String");
        leee::ByteArray copy = original;
        CHECK(copy.str() == "Test String");
        CHECK(copy.size() == original.size());
    }
}

// 测试用例：字符串操作
TEST_CASE("ByteArray - String Operations") {
    SUBCASE("Substring operations") {
        leee::ByteArray ba("Hello World");

        // left()
        leee::ByteArray left = ba.left(5);
        CHECK(left.str() == "Hello");

        // right()
        leee::ByteArray right = ba.right(5);
        CHECK(right.str() == "World");

        // mid()
        leee::ByteArray mid = ba.mid(6, 5);
        CHECK(mid.str() == "World");
    }

    SUBCASE("String checking") {
        leee::ByteArray ba("Hello World");

        CHECK(ba.startsWith("Hello"));
        CHECK_FALSE(ba.startsWith("World"));

        CHECK(ba.endsWith("World"));
        CHECK_FALSE(ba.endsWith("Hello"));

        CHECK(ba.contains("lo Wo"));
        CHECK_FALSE(ba.contains("xyz"));
    }

    SUBCASE("Case conversion") {
        leee::ByteArray ba("Hello World");

        ba.toLower();
        CHECK(ba.str() == "hello world");

        ba.toUpper();
        CHECK(ba.str() == "HELLO WORLD");
    }

    SUBCASE("Trim operations") {
        leee::ByteArray ba("  Hello World  ");

        ba.trim();
        CHECK(ba.str() == "Hello World");

        leee::ByteArray left_space("  Hello");
        left_space.trimLeft();
        CHECK(left_space.str() == "Hello");

        leee::ByteArray right_space("Hello  ");
        right_space.trimRight();
        CHECK(right_space.str() == "Hello");
    }
}

// 测试用例：字符串修改
TEST_CASE("ByteArray - String Modification") {
    SUBCASE("Append operations") {
        leee::ByteArray ba("Hello");

        ba.append(" World");
        CHECK(ba.str() == "Hello World");

        ba.append('!');
        CHECK(ba.str() == "Hello World!");
    }

    SUBCASE("Prepend operations") {
        leee::ByteArray ba("World");

        ba.prepend("Hello ");
        CHECK(ba.str() == "Hello World");
    }

    SUBCASE("Replace operation") {
        leee::ByteArray ba("Hello World");

        ba.replace("World", "Universe");
        CHECK(ba.str() == "Hello Universe");

        ba.replace("l", "L");
        CHECK(ba.str() == "HeLLo Universe");
    }

    SUBCASE("Insert and remove") {
        leee::ByteArray ba("Hello");

        ba.insert(5, " World");
        CHECK(ba.str() == "Hello World");

        ba.remove(5, 6);
        CHECK(ba.str() == "Hello");
    }
}

// 测试用例：字符串分割和连接
TEST_CASE("ByteArray - Split and Join") {
    SUBCASE("Split by character") {
        leee::ByteArray ba("Hello,World,Test");
        auto parts = ba.split(',');

        CHECK(parts.size() == 3);
        CHECK(parts[0].str() == "Hello");
        CHECK(parts[1].str() == "World");
        CHECK(parts[2].str() == "Test");
    }

    SUBCASE("Split by string") {
        leee::ByteArray ba("Hello -> World -> Test");
        auto parts = ba.split(" -> ");

        CHECK(parts.size() == 3);
        CHECK(parts[0].str() == "Hello");
        CHECK(parts[1].str() == "World");
        CHECK(parts[2].str() == "Test");
    }

    SUBCASE("Join operation") {
        std::vector<leee::ByteArray> parts = {"Hello", "World", "Test"};
        leee::ByteArray joined = leee::ByteArray::join(parts, ", ");

        CHECK(joined.str() == "Hello, World, Test");
    }
}

// 测试用例：查找操作
TEST_CASE("ByteArray - Search Operations") {
    leee::ByteArray ba("Hello World Hello");

    SUBCASE("Index of character") {
        CHECK(ba.indexOf('H') == 0);
        CHECK(ba.indexOf('l') == 2);
        CHECK(ba.indexOf('z') == -1);
    }

    SUBCASE("Index of string") {
        CHECK(ba.indexOf("World") == 6);
        CHECK(ba.indexOf("Hello") == 0);
        CHECK(ba.indexOf("xyz") == -1);
    }

    SUBCASE("Last index of") {
        CHECK(ba.lastIndexOf('H') == 12);
        CHECK(ba.lastIndexOf("Hello") == 12);
    }
}

// 测试用例：类型转换
TEST_CASE("ByteArray - Type Conversion") {
    SUBCASE("Integer conversion") {
        leee::ByteArray ba("42");

        bool ok;
        int value = ba.toInt(&ok);
        CHECK(ok);
        CHECK(value == 42);

        leee::ByteArray invalid("abc");
        int invalid_value = invalid.toInt(&ok);
        CHECK_FALSE(ok);
        CHECK(invalid_value == 0);
    }

    SUBCASE("Floating point conversion") {
        leee::ByteArray ba("3.14159");

        bool ok;
        double value = ba.toDouble(&ok);
        CHECK(ok);
        CHECK(value == doctest::Approx(3.14159));

        float fvalue = ba.toFloat(&ok);
        CHECK(ok);
        CHECK(fvalue == doctest::Approx(3.14159f));
    }

    SUBCASE("Number to string") {
        leee::ByteArray str_int = leee::ByteArray::number(42);
        CHECK(str_int.str() == "42");

        leee::ByteArray str_double = leee::ByteArray::number(3.14, 'f', 2);
        CHECK(str_double.str() == "3.14");

        leee::ByteArray str_hex = leee::ByteArray::number(255, 16);
        CHECK(str_hex.str() == "0xff");
    }
}

// 测试用例：文件操作
TEST_CASE("ByteArray - File Operations") {
    const std::string test_file = "test_bytearray.txt";
    const std::string test_content = "Hello, this is test content!\nWith multiple lines.";

    SUBCASE("Save and load file") {
        leee::ByteArray ba(test_content);

        // Save to file
        bool save_result = ba.saveToFile(test_file);
        CHECK(save_result);

        // Load from file
        leee::ByteArray loaded = leee::ByteArray::loadFromFile(test_file);
        CHECK(loaded.str() == test_content);

        // Clean up
        std::remove(test_file.c_str());
    }

    SUBCASE("Append to file") {
        leee::ByteArray ba1("First line");
        leee::ByteArray ba2("Second line");

        ba1.saveToFile(test_file);
        ba2.appendToFile(test_file);

        leee::ByteArray loaded = leee::ByteArray::loadFromFile(test_file);
        CHECK(loaded.str() == "First lineSecond line");

        // Clean up
        std::remove(test_file.c_str());
    }
}

// 测试用例：路径操作
TEST_CASE("ByteArray - Path Operations") {
    SUBCASE("File name extraction") {
        leee::ByteArray path("/home/user/documents/test.txt");

        leee::ByteArray fileName = leee::ByteArray::getFileName(path.str());
        CHECK(fileName.str() == "test.txt");

        leee::ByteArray directory = leee::ByteArray::getDirectory(path.str());
        CHECK(directory.str() == "/home/user/documents/");

        leee::ByteArray extension = leee::ByteArray::getFileExtension(path.str());
        CHECK(extension.str() == ".txt");
    }

    SUBCASE("Instance methods") {
        leee::ByteArray path("/home/user/test.cpp");

        CHECK(path.getFileName().str() == "test.cpp");
        CHECK(path.getDirectory().str() == "/home/user/");
        CHECK(path.getFileExtension().str() == ".cpp");
    }
}

// 测试用例：编码操作
TEST_CASE("ByteArray - Encoding Operations") {
    SUBCASE("Hex encoding") {
        leee::ByteArray ba("Hello");
        leee::ByteArray hex = ba.toHexString();

        // "Hello" in hex should be "48656c6c6f"
        CHECK(hex.str() == "48656c6c6f");

        leee::ByteArray decoded = leee::ByteArray::fromHexString(hex.str());
        CHECK(decoded.str() == "Hello");
    }

    SUBCASE("Invalid hex string") {
        leee::ByteArray invalid = leee::ByteArray::fromHexString("invalid");
        CHECK(invalid.isEmpty());
    }
}

// 测试用例：比较操作
TEST_CASE("ByteArray - Comparison Operations") {
    leee::ByteArray ba1("Hello");
    leee::ByteArray ba2("Hello");
    leee::ByteArray ba3("World");

    CHECK(ba1 == ba2);
    CHECK(ba1 != ba3);
    CHECK(ba1 < ba3);
    CHECK(ba3 > ba1);

    // Case sensitive comparison
    leee::ByteArray lower("hello");
    leee::ByteArray upper("HELLO");

    CHECK(ba1.compare(upper, false) == 0); // Case insensitive
    CHECK(ba1.compare(upper, true) != 0);  // Case sensitive
}

// 测试用例：工具函数
TEST_CASE("ByteArray - Utility Functions") {
    SUBCASE("Fill operation") {
        leee::ByteArray ba;
        ba.fill('A', 5);
        CHECK(ba.str() == "AAAAA");
        CHECK(ba.size() == 5);
    }

    SUBCASE("Repeated operation") {
        leee::ByteArray ba("Hi");
        leee::ByteArray repeated = ba.repeated(3);
        CHECK(repeated.str() == "HiHiHi");
    }

    SUBCASE("Timestamp functions") {
        long long ticks = leee::ByteArray::getCurrentTimeStampTicks();
        CHECK(ticks > 0);

        double ms = leee::ByteArray::getCurrentTimeStampTicksMs();
        CHECK(ms > 0);

        leee::ByteArray timeStr = leee::ByteArray::getCurrentTimeString();
        CHECK_FALSE(timeStr.isEmpty());
    }
}

// 测试用例：运算符重载
TEST_CASE("ByteArray - Operators") {
    leee::ByteArray ba1("Hello");
    leee::ByteArray ba2(" World");

    SUBCASE("Concatenation") {
        leee::ByteArray result = ba1 + ba2;
        CHECK(result.str() == "Hello World");

        result += "!";
        CHECK(result.str() == "Hello World!");
    }

    SUBCASE("Assignment") {
        leee::ByteArray copy;
        copy = ba1;
        CHECK(copy.str() == "Hello");
    }
}

// 测试用例：数据打包和解包
TEST_CASE("ByteArray - Data Packing") {
    SUBCASE("Pack and unpack integer") {
        int original = 42;
        leee::ByteArray packed = leee::ByteArray::pack(original);

        CHECK(packed.size() == sizeof(int));

        bool ok;
        int unpacked = packed.unpack<int>(&ok);
        CHECK(ok);
        CHECK(unpacked == original);
    }

    SUBCASE("Pack and unpack double") {
        double original = 3.14159;
        leee::ByteArray packed = leee::ByteArray::pack(original);

        CHECK(packed.size() == sizeof(double));

        double unpacked = packed.unpack<double>();
        CHECK(unpacked == doctest::Approx(original));
    }
}

// 测试用例：性能分析工具
TEST_CASE("ByteArray - Performance Util") {
    leee::PerformanceUtil perf;

    perf.reset(); // reset()会记录一个初始时间点

    // 模拟一些操作
    for (int i = 0; i < 100; ++i) {
        leee::ByteArray temp = leee::ByteArray("test").toUpper();
    }

    perf.recordTimePoint(); // 再记录一个时间点

    auto intervals = perf.getIntervals();
    CHECK(intervals.size() == 1); // 两个时间点产生1个间隔
    CHECK(intervals[0] >= 0.0);

    double duration = perf.getDuration();
    CHECK(duration >= 0.0);

    leee::ByteArray report = perf.getReport();
    CHECK_FALSE(report.isEmpty());

    // 测试多个时间点
    perf.recordTimePoint();
    auto intervals2 = perf.getIntervals();
    CHECK(intervals2.size() == 2); // 三个时间点产生2个间隔
}
