//
// Leee Logger Module Example
// 演示如何使用 Leee 项目的日志模块
//

#include <logger.h>
#include <iostream>
#include <vector>
#include <string>

int main()
{
    // 初始化日志模块
    leee::init_logger();

    std::cout << "=== Leee Logger Example ===" << std::endl;

    // 基础日志记录
    LOG_INFO << "Logger initialized successfully";
    LOG_WARNING << "This is a warning message";
    LOG_ERROR << "This is an error message";

    // 流式日志接口示例
    int value = 42;
    std::string name = "Leee";
    LOG_INFO << "Processing value: " << value << " for " << name;

    // 记录向量数据
    std::vector<double> data = {1.1, 2.2, 3.3, 4.4, 5.5};
    LOG_INFO << "Data vector: [" << data.size() << " elements]";

    // 不同日志级别的使用
    LOG_DEBUG << "Debug information";
    LOG_VERBOSE << "Verbose information";

    // 使用不同的日志文件路径
    // leee::init_logger("custom_logs/my_app.log", 2000000, 10); // 2MB, 10 files

    // 检查日志级别
    LOG_INFO << "Current log level: " << static_cast<int>(leee::get_log_level());

    // 设置日志级别
    leee::set_log_level(plog::info);
    LOG_INFO << "Log level set to INFO";

    LOG_INFO << "Logger example completed";

    return 0;
}
